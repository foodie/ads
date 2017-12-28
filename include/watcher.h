#ifndef _WATCHER_H_
#define _WATCHER_H_

#include <sys/time.h>
#include <sys/stat.h>
#include <string>

// We often want to know if a file was just now created or updated (then we
// may reload the file to get latest data). FileWatcher does boilerplate part
// for you as below.
// Example:
//   Watcher fw;
//   fw.create ("path_of_file_to_be_watched");
//   ....
//   if (fw.is_timestamp_updated() > 0) {  // >0 means that the file is created or updated 
//       ......
//       fw.update_timestamp(); // update recorded timestamp
//   }
//
// You may call is_timestamp_updated() multiple times to know latest status
// of the file before calling update_timestamp(), or you may combine calls to
// the two functions by:
//   if (fw.check_and_update_timestamp() > 0) {
//       ....
//   }

class Watcher {
public:
    static const time_t NON_EXIST_TS = (time_t)-1;
    // must be negative
    static const int SKEW = -2;
    static const int DELETED = -1;
    // must be zero
    static const int UNCHANGED = 0;
    // must be positive
    static const int UPDATED = 1; 
    static const int CREATED = 2;
    
    // Default constructor
    explicit Watcher ()
        : _chk_ts(NON_EXIST_TS) , _last_ts(NON_EXIST_TS)
    {}

    // Create this watcher on a file
    // Returns:
    //   -1   file_path is NULL
    //    0   success
    int create (const char* file_path)
    {
        if (NULL == file_path) {
            FATAL("file_path is NULL");
            return -1;
        }
        
        file_path_ = file_path;

        struct stat tmp_st;
        int ret = stat (file_path_.c_str(), &tmp_st);
        if (ret < 0) {
            WARN("%s does not exist", file_path_.c_str());
            _chk_ts = NON_EXIST_TS;
            _last_ts = NON_EXIST_TS;
        } else {
            _chk_ts = tmp_st.st_mtime;
            _last_ts = tmp_st.st_mtime;
        }
        return 0;
    }

    // Check if watched file was created/modified/deleted. Recorded timestamp
    // is not changed so it's safe to call this function multiple times, you
    // won't miss an update.
    // Returns:
    //   UPDATED   the file is modified or created since last call to this
    //             function
    //   UNCHANGED the file is not modified or still not existing since last
    //             call to this function
    //   DELETED   the file was deleted since last call to this function
    //   SKEW      the file exists but the timestamp steps back
    // Note: this method only checks, to tell file_reader_t to forget the change
    //       (because you already processed the file), you must call
    //       update_timestamp(). You may call check_and_update_timestamp() to
    //       combine two calls.
    // Note: If the file is updated too frequently(< 500ms), this method may
    //       return UNCHANGED due to precision of stat(2); If the file is
    //       being updated too frequently for a period of time, this method
    //       should eventually return several UPDATEs; If the file is being 
    //       updated and deleted too frequently, the update events are
    //       probably undetectable. Fortunately, this is unseen in our app.
    int is_timestamp_updated ()
    {
        struct stat tmp_st;
        const int ret = stat (file_path_.c_str(), &tmp_st);
        if (ret < 0) {
            _chk_ts = NON_EXIST_TS;
            if (NON_EXIST_TS != _last_ts) {
                return DELETED;
            } else {
                return UNCHANGED;
            }
        } else {
            _chk_ts = tmp_st.st_mtime;
            if (NON_EXIST_TS != _last_ts) {
                if (_chk_ts > _last_ts) {
                    return UPDATED;
                } else if (_chk_ts == _last_ts) {
                    return UNCHANGED;
                } else {
                    return SKEW;
                }
            } else {
                return CREATED;
            }
        }
    }

    // Change recorded time
    void update_timestamp ()
    {
        _last_ts = _chk_ts;
    }

    // Combine is_timestamp_updated() and update_timestamp()
    int check_and_update_timestamp ()
    {
        const int ret = is_timestamp_updated ();
        update_timestamp ();
        return ret;
    }
    
    // Get path of the watched file
    const char* filepath () const {
        return file_path_.c_str(); 
    }

private:
    std::string file_path_;
    time_t _chk_ts;
    time_t _last_ts;    
};

typedef Watcher watcher_t;

#endif
