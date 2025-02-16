#include "SdFatPlayList.h"

int modulo(int a, int b) {
    return a >= 0 ? a % b : ( b - abs(a % b)) % b; 
}

bool fileFilter::operator()(File32& file) const {
    char short_fn[13]; 
    const char *p;    
    int len;
    if (v_ext.empty()) 
        return true; 
    // the filefilter uses short filenames: only the first 3 characters of file extension will be tested
    len = file.getSFN(short_fn, sizeof(short_fn));
    while (len)
        if (short_fn[--len] == '.') {
        p = short_fn + len + 1;
        for ( String ext : v_ext ) {
            if ( strcasecmp(p, ext.c_str()) == 0) 
                return true;
        }
    }
    return false;
}

void SdFatPlayList::setFileFilter(const std::initializer_list<const char*> iLst_ext) {
    files.clear();
    dirs.clear();
    is_valid_File.init(iLst_ext);
}

const char* SdFatPlayList::name(File32& f, bool add_dirslash) {
    static char m_path[256+1];
    int i = 0;
    if ( add_dirslash ) 
        m_path[i++] = '/';
    m_path[i] = '\0';
    f.getName(m_path+i, sizeof(m_path)-i);
    return (const char*)m_path;
}

int SdFatPlayList::createPlayList(const char *path, uint8_t levels) {    
    std::vector<String> sub_dirs;
    String sub_dir_path;
    int dirs_index = dirs.size();
    int file_count = 0;   // count files of dir
    
    if (!cur_dir.open(path)) {
        log_e("%s open failed", path);
        return 0;
    }
    while (cur_dir) {
        if ( cur_dirfile.openNext(&cur_dir) ) {
            if ( !(cur_dirfile.isSystem() || cur_dirfile.isHidden()) ) {    // hidden: filename starts with \.
                if (cur_dirfile.isDir()) {
                    if (levels) {
                        sub_dirs.emplace_back(name(cur_dirfile, !cur_dir.isRoot()));
                        log_d("subdirs.push: levels: %d, path: %s", levels, sub_dirs.back().c_str());
                    }
                }
                else {
                    if ( is_valid_File(cur_dirfile) )  {
                        #if SAVE_FNAMES
                            files.emplace_back(dirs_index, cur_dirfile.dirIndex(), name(cur_dirfile)); 
                        #else
                            files.push_back({dirs_index, cur_dirfile.dirIndex()}); 
                        #endif
                        file_count++;
                    }
                }
            }
            cur_dirfile.close();
        }
        else 
            break;
    }
    cur_dir.close();
    if (file_count) 
        dirs.emplace_back(path);
    if (levels) {
        for ( auto e : sub_dirs ) {
            (sub_dir_path = path) += e;
            log_d("call listDir: levels: %d, path: = %s, dirs_size = %d, files: %d", levels, sub_dir_path.c_str(), dirs.size(), files.size());            
            createPlayList(sub_dir_path.c_str(), levels-1);
        }
    }
    return files.size();
}

const char* SdFatPlayList::getFilePathAtIdx(size_t idx) {
    log_d("get: file[%d] - dirs[%d]", idx, files[idx].dirs_index);
    if ( idx < files.size()) { 
        cur_path = dirs[files[idx].dirs_index].path;
    #if SAVE_FNAMES
        if (cur_path != "/" ) cur_path += '/';
        cur_path +=  files[idx].name;
    #else        
        if ( cur_dir.open(cur_path.c_str()) ) {
            if ( cur_dirfile.open(&cur_dir, files[idx].fat_dir_index) ) {
                if (!cur_dir.isRoot()) cur_path += '/';
                cur_path += name(cur_dirfile);
                cur_dirfile.close();
            }
            cur_dir.close();
        }
    #endif
        return cur_path.c_str();
    }
    return nullptr;
}