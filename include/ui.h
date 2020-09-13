#include "mdsystem.h"
#define MDVERSION "V1.1(By geruipeng)"
/*
	in "main" function, an object of MdSystem will be created.
	So in all the UI function, the address of the object should 
	always be an argument in order to implement all the operations.
*/
//the main screen
void enter_main(MdSystem* ms);
//the add_new screen
void enter_addnew(MdSystem *ms);
//the view_all_file screen
void enter_display(MdSystem* ms);
//the exit screen
void enter_exit(MdSystem* ms);
//add lines from the last.
void edit_file(MdSystem* ms, MdFile* mf);
//display file 
void view_file(MdSystem* ms, MdFile* mf);
//add a line in the middle
void add_mid_file(MdSystem* ms, MdFile* mf);
//modify an existing line
void modify_file(MdSystem* ms, MdFile* mf);
//delete an existing line
void delete_line_file(MdSystem* ms, MdFile* mf);
//translate into html
void enter_trans(MdSystem* ms, MdFile* mf);

void enter_export_md(MdSystem* ms, MdFile* mf);
