/* Ryan Kadri, Austin Su, Nyambi Acho, Utkarsha Mavani*/
#include "mathfs.h"

/* Takes the global arrays and combines them together into operation structures*/
int loadOperations(){
	
	int i;
	for(i = 0; i < numOps; ++i){
		myOperations[i].path = paths[i];
		myOperations[i].doc = docs[i];
		myOperations[i].numArgs = args[i];
	}
	
	myOperations[0].parse = &opAdd;
	myOperations[1].parse = &opSub;
	myOperations[2].parse = &opMul;
	myOperations[3].parse = &opDiv;
	myOperations[4].parse = &opExp;
	myOperations[5].parse = &opFib;
	myOperations[6].parse = &opFactor;
	
	return 0;
}

/* Matches the first characters of input with prefix*/
int prefixMatch(const char* input, const char* prefix){
	
	if(strlen(prefix) > strlen(input)){
		return -1; /* prefix is longer than input. No match*/
	}
	
	int i;
	for(i = 0; prefix[i] != '\0'; ++i){
		if(input[i] != prefix[i]){
			return -1; /* prefix does not match*/
		}
	}
	
	if(input[i] == '\0' && prefix[i] == '\0'){
		return 0; /* input and prefix exactly match*/
	}else{
		return i; /* otherwise return the length of prefix*/
	}
	
}

/* Returns the "type" of a file. See header. */
int getFileType(const char* path){
	
	struct operation thisOp;
	 
	if(getOp(path) == NULL){
		return -ENOENT;
	}else{
		thisOp = *getOp(path);
	}
		
	int res = prefixMatch(path, thisOp.path);
	
	if(res < 0){
		return INVALID; /* Not a valid file */
		
	}else if(res == 0){
		return PATH_DIR_HARD; /* One of the hardcoded directories (/add, /factor, etc.)*/
		
	}else if(strcmp(path + res, "/doc")==0){
		return PATH_FILE_HARD; /* A hardcoded file (Documentation)*/
		
	}else if(path[res]=='/'){
		if(thisOp.numArgs == 2){ /* If a function has multiple arguments, it can have a subdirectory*/
			if(strchr(path + res + 1,'/') != NULL){
				return PATH_FILE_SOFT; /* This is a dynamically created file (eg. add/2/1) */
			}else{
				return PATH_DIR_SOFT; /* This is a dynamically created directoy (eg. add/2) */
			}
			
		}else{ /* If the operation only has one argument, a path for that
				  argument will be a file*/
			return PATH_FILE_SOFT;
		}
		
	}else{
		return INVALID; /* Not a valid file*/
	}
}

/* Return a pointer to the operation in path */
struct operation* getOp(const char* path){
	
	int i;
	for(i = 0; i < numOps; ++i){
		if(prefixMatch(path,myOperations[i].path)>=0){
			return &myOperations[i];
		}
	}
	
	return NULL;
} 

// FUSE function implementations.
static int mathfs_getattr(const char *path, struct stat *stbuf)
{
	
	memset(stbuf, 0, sizeof(struct stat));
	
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
	}
		
	struct operation thisOp;
	if(getOp(path) == NULL){
		return -ENOENT;
	}else{
		thisOp = *getOp(path);
	}
	int res = getFileType(path);
	
	/* Treat directories the same */
	if(res == PATH_DIR_HARD || res == PATH_DIR_SOFT){
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
		
	/* Main directories contain read only documentation files*/
	}else if(res == PATH_FILE_HARD){
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(thisOp.doc);
		return 0;
	
	/* Dynamic files are created read-only and with a dynamic size*/
	}
	else if(res == PATH_FILE_SOFT){
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 1;
			int res = prefixMatch(path,thisOp.path);
			stbuf->st_size = strlen(thisOp.parse(path+res));
			return 0;
	}else{
		return -ENOENT;
	}
}

static int mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;
    
    /* Relative root path contains all main directories */
    if (strcmp(path, "/") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		int i;
		for(i = 0; i < numOps; ++i){
			filler(buf, myOperations[i].path + 1, NULL, 0);
		}
		return 0;
	}
	
	/* Main directories contain documentation*/
	else if(getFileType(path)==0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
		return 0;
	
	/* Dynamically created directories contain basic links*/
	}else if(getFileType(path) == 2){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		return 0;
		
	}else{
		return -ENOENT;
	}
}		

static int mathfs_open(const char *path, struct fuse_file_info *fi){
	
	/* A file can be opened if it has a valid path*/
	if(getFileType(path) != INVALID){
		if ((fi->flags & 3) != O_RDONLY)
			return -EACCES;
	}else{
		return -ENOENT;
	}
	
	return 0;
}

static int mathfs_read(const char *path, char *buf, size_t size, off_t offset,
						struct fuse_file_info *fi)
{
    (void) fi;
    struct operation thisOp;
    char* contents;
    
    if(getOp(path) != NULL){
		thisOp = * getOp(path);
	}else{
		return -ENOENT;
	}
	
	int type = getFileType(path);

	/* This type of file is always doc in this FS*/
	if(type == PATH_FILE_HARD){
		contents = thisOp.doc;
		
	/* A dynamically generated file must have its contents generated*/
	}else if(type == PATH_FILE_SOFT){
		int res = prefixMatch(path,thisOp.path);
		contents = thisOp.parse(path + res);
		if(contents == NULL){
			return -ENOENT;
		}
	}else{
		return -ENOENT;
	}
	
	int len = strlen(contents);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, contents + offset, size);
	} else {
		size = 0;
	}
	
	if(type == 3) free(contents);
	return size;

}

static struct fuse_operations mathfs_oper = {
    .getattr = mathfs_getattr,
    .readdir = mathfs_readdir,
    .open = mathfs_open,
    .read = mathfs_read,
};

int main(int argc, char **argv){
    loadOperations();
    return fuse_main(argc, argv, &mathfs_oper, NULL);
}
