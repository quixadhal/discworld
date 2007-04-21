#ifndef __project_management_h
#define __project_management_h

#define MINUTE (60) 
#define HOUR (60 * 60)
#define DAY (HOUR * 24)
#define WEEK (DAY * 7)

#define PROJECT_LOCK (5 * MINUTE)
#define TIME_BETWEEN_POSTINGS (7 * DAY)

#define STATUS ({"heap", "development", "playtesting", "play", "limbo"})
#define SIZE ({"very small", "small", "medium", "large", "very large", "huge", "special"})
#define COMPLEXITY ({"low", "medium", "high", "very high", "special"})
#define PROJECT_SAVE "/save/project"
#define TMP_SAVE "/save/project_tmp"
#define PROJECT_LOG "PROJECT_MANAGEMENT"
#define PROJECT_HANDLER "/obj/handlers/project_management"
#define TWIKI_BASE "http://discworld.imaginary.com/twiki/bin/view/"
#define WHATWHAT ("/www/whatwhat")

#define PROJECT "project"
#define TASK "task"
#define NOTE "note"
#define FILE "file"
#define PLAYTESTING "playtesting"
#define CALLBACK "project_callback"

class task {
  string id; 
  string task_name;
  string comments;
  string *assigned_to;
  string added_by;
  string aims;
  int percent_complete;
  int completion_date;
  int date_added;
}

class note {
  string id;
  int date_added;
  string added_by;
  string note;
}

class file {
  string file;
  int access;
  string task;
}


class project {
  string id;
  string name;
  string leader;
  string added_by;
  string description;
  string pt_notes;  
  string what_touched;
  string touched_by;

  int date_added;
  int date_completed;
  int status;
  int percent_complete;
  int estimated_completion;
  int last_touched;
  int last_status_change;  

  string *twiki;
  string *playtesters;
  string *domains;
  string *creators;
  string *sub_projects;  

  class file *files;
  class task *tasks;
  class note *notes;

  int last_rotation;
  int size;
  int complexity;
  int current_rotation;
  string *last_pt_cohort;
  string *all_pts;
  string *guild_specific;

}


class project_tmp {
  string id;
  string name;
  string leader;
  string added_by;
  string description;
  string pt_notes;  
  string *domains;
  string *creators;
  string *sub_projects;  
  class file *files;
  string *twiki;
  string *playtesters;
  string what_touched;
  string touched_by;
  int date_added;
  int date_completed;
  int status;
  int percent_complete;
  int estimated_completion;
  int last_touched;
  int last_status_change;  
  class task *tasks;
  class note *notes;
}

#endif
