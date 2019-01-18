.DT
ftp.h
Disk World autodoc help
ftp.h

.SH Classes

.SI 3
* %^BOLD%^dataconn%^RESET%^
class dataconn {
mixed data;
string path;
int pos;
int parent_fd;
int type;
int len;
int append;
}

.EI

.SI 3
* %^BOLD%^session%^RESET%^
class session {
int fd;
string user_name;
int logged_in;
string cwd;
string data_addr;
int last_data;
int data_port;
int data_fd;
int type;
int idle;
string rnfr;
int offset;
int use_default;
int pasv_fd;
int pasv_cb;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^BINARY%^RESET%^
.EI

.SI 3
* %^BOLD%^BLOCK_SIZE%^RESET%^
.EI

.SI 3
* %^BOLD%^CHECK_CMD(INT)%^RESET%^
.EI

.SI 3
* %^BOLD%^CHECK_CMD2(INT,INT2)%^RESET%^
.EI

.SI 3
* %^BOLD%^CHECK_LOGIN()%^RESET%^
.EI

.SI 3
* %^BOLD%^CHECK_PLAYER()%^RESET%^
.EI

.SI 3
* %^BOLD%^CHECK_STRING%^RESET%^
.EI

.SI 3
* %^BOLD%^DEBUG_RECEIVE%^RESET%^
.EI

.SI 3
* %^BOLD%^DEBUG_SEND%^RESET%^
.EI

.SI 3
* %^BOLD%^DOWNLOAD%^RESET%^
.EI

.SI 3
* %^BOLD%^FILE%^RESET%^
.EI

.SI 3
* %^BOLD%^FTP_PORT%^RESET%^
.EI

.SI 3
* %^BOLD%^FTP_VERSION%^RESET%^
.EI

.SI 3
* %^BOLD%^HOME_DIR(NAME)%^RESET%^
.EI

.SI 3
* %^BOLD%^LOG_CD_SIZE%^RESET%^
.EI

.SI 3
* %^BOLD%^LOG_CONNECT%^RESET%^
.EI

.SI 3
* %^BOLD%^LOG_DEBUG%^RESET%^
.EI

.SI 3
* %^BOLD%^LOG_FILE%^RESET%^
.EI

.SI 3
* %^BOLD%^LOG_FILENAME%^RESET%^
.EI

.SI 3
* %^BOLD%^LOGIN_OB%^RESET%^
.EI

.SI 3
* %^BOLD%^MASK_A%^RESET%^
.EI

.SI 3
* %^BOLD%^MASK_C%^RESET%^
.EI

.SI 3
* %^BOLD%^MASK_D%^RESET%^
.EI

.SI 3
* %^BOLD%^MASK_F%^RESET%^
.EI

.SI 3
* %^BOLD%^MASK_L%^RESET%^
.EI

.SI 3
* %^BOLD%^MASK_R%^RESET%^
.EI

.SI 3
* %^BOLD%^STRING%^RESET%^
.EI

.SI 3
* %^BOLD%^TP(STR)%^RESET%^
.EI

.SI 3
* %^BOLD%^TP_CRE%^RESET%^
.EI

.SI 3
* %^BOLD%^UNAME%^RESET%^
.EI

.SI 3
* %^BOLD%^VERSION%^RESET%^
.EI

