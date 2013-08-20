
#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <iostream>


void copyData(archive* file, archive* dest) {
	int status;
	const void* buff;
	size_t size;
#if ARCHIVE_VERSION_NUMBER < 3000000
	off_t offset;
#else
	int64_t offset;
#endif


	while(true) {
		status=archive_read_data_block(file, &buff, &size, &offset);
		if(status==ARCHIVE_EOF){
			return;
		}
		if(status!=ARCHIVE_OK){
			std::cerr<<"ERROR: An error occurred while writing data to disk."<<std::endl;
			return;
		}
		status=archive_write_data_block(dest, buff, size, offset);
		if(status!=ARCHIVE_OK) {
			std::cerr<<"ERROR: An error occurred while writing data to disk."<<std::endl;
			return;
		}
	}
}

static int copy_data(struct archive *ar, struct archive *aw)
{
  int r;
  const void *buff;
  size_t size;
  off_t offset;

  for (;;) {
    r = archive_read_data_block(ar, &buff, &size, &offset);
    if (r == ARCHIVE_EOF)
      return (ARCHIVE_OK);
    if (r != ARCHIVE_OK)
      return (r);
    r = archive_write_data_block(aw, buff, size, offset);
    if (r != ARCHIVE_OK) {
      fprintf(stderr, "%s\n", archive_error_string(aw));
      return (r);
    }
  }
}

bool extractFile(const std::string &fileName, const std::string &destination) {
	//Create the archive we're going to extract.
	archive* file;
	//Create the destination we're going to extract to.
	archive* dest;
	
	file=archive_read_new();
	dest=archive_write_disk_new();
	archive_write_disk_set_options(dest, ARCHIVE_EXTRACT_TIME);
	
	archive_read_support_format_zip(file);
	
	//Now read the archive.
	if(archive_read_open_file(file,fileName.c_str(),10240)) {
		std::cerr<<"ERROR: An error occurred while reading archive "+fileName<<std::endl;
		return false;
	}
	
	//Now write every entry to disk.
	int status;
	archive_entry* entry=NULL;
	while(true) {
		status=archive_read_next_header(file,&entry);
		if(status==ARCHIVE_EOF){
			break;
		}
		if(status!=ARCHIVE_OK){
			std::cerr<<"ERROR: An error occurred while reading archive "+fileName<<std::endl;
			return false;
		}
		archive_entry_copy_pathname(entry,(destination+archive_entry_pathname(entry)).c_str());
		
		status=archive_write_header(dest,entry);
		if(status!=ARCHIVE_OK){
			std::cerr<<"ERROR: An error occurred while extracting archive "+fileName<<std::endl;
			return false;
		}else{
			copyData(file, dest);
			status=archive_write_finish_entry(dest);
			if(status!=ARCHIVE_OK){
				std::cerr<<"ERROR: An error occurred while extracting archive "+fileName<<std::endl;
				return false;
			}

		}
	}
	
	//Finally close the archive.
	archive_read_close(file);
	archive_read_finish(file);
	return true;
}

static void extract(const char *filename)
{
  struct archive *a;
  struct archive *ext;
  struct archive_entry *entry;
  int flags;
  int r;

  /* Select which attributes we want to restore. */
  flags = ARCHIVE_EXTRACT_TIME;
  flags |= ARCHIVE_EXTRACT_PERM;
  flags |= ARCHIVE_EXTRACT_ACL;
  flags |= ARCHIVE_EXTRACT_FFLAGS;

  a = archive_read_new();
  ext = archive_write_disk_new();
  archive_read_support_format_all(a);
  archive_read_support_compression_all(a);
  archive_write_disk_set_options(ext, flags);
  archive_write_disk_set_standard_lookup(ext);
  if ((r = archive_read_open_filename(a, filename, 10240)))
    exit(1);
  for (;;) {
    r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF)
      break;
    if (r != ARCHIVE_OK)
      fprintf(stderr, "%s\n", archive_error_string(a));
    if (r < ARCHIVE_WARN)
      exit(1);
    r = archive_write_header(ext, entry);
    if (r != ARCHIVE_OK)
      fprintf(stderr, "%s\n", archive_error_string(ext));
    else if (archive_entry_size(entry) > 0) {
      copy_data(a, ext);
      if (r != ARCHIVE_OK)
        fprintf(stderr, "%s\n", archive_error_string(ext));
      if (r < ARCHIVE_WARN)
        exit(1);
    }
    r = archive_write_finish_entry(ext);
    if (r != ARCHIVE_OK)
      fprintf(stderr, "%s\n", archive_error_string(ext));
    if (r < ARCHIVE_WARN)
      exit(1);
  }
  archive_read_close(a);
  archive_read_free(a);
  archive_write_close(ext);
  archive_write_free(ext);
  exit(0);
}

void list_archive (const char* file){
	
	struct archive *a;
	struct archive_entry *entry;
	int r;

	a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_all(a);
	r = archive_read_open_filename(a, file, 10240); // Note 1
	if (r != ARCHIVE_OK)
	  exit(1);
	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
	  printf("%s\n",archive_entry_pathname(entry));
	  archive_read_data_skip(a);  // Note 2
	}
	r = archive_read_free(a);  // Note 3
	if (r != ARCHIVE_OK)
	  exit(1);
}
