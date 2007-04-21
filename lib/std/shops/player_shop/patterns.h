/* Office chart command */
#define CHART_ADDITEM       "add <string'item'>"
#define CHART_REMITEM       "remove <string'item'>"
#define CHART_MAXITEM       "max <string'item'> <number>"
#define CHART_BUYITEM       "buy <string'item'> <number>"
#define CHART_SELLITEM      "sell <string'item'> <number>"
#define CHART_ASSIGN_ITEM   "assign <string'item'> <number'cabinet number'>"
#define CHART_UNASSIGN_ITEM "unassign <string'item'> <number'cabinet number'>"

/* Offices logs command */
#define LOG_BLANK  ""
#define LOG_NUMBER "<number>"
#define LOG_MGR    "{personnel|accounts|chart}"
#define LOG_CHART  "chart"

/* Mgr Office list command */
#define LIST_BLANK  ""
#define LIST_CHOICE "{employees|applicants|baddies}"

/* Mgr Office set command */
#define SET_EMPS "maximum <number> [employees]"
#define SET_PAY  "pay <number'amount'> <string'type'>"

/* Mgr Office vote command */
#define VOTE_APPLICANT "<word'applicant'> {y|n|a}"
#define VOTE_POLICY    "policy <word'name'> {y|n}"

/* Mgr Office view command */
#define VIEW_EMP "<string'employee'>"
#define VIEW_APP "application [by] <string'applicant'>"

/* Mgr Office policy command */
#define POLICY_SUGGEST "suggest {manager|shop} <word'policy'>"
#define POLICY_REMOVE  "remove <word'policy'>"

/* Stockroom list command */
#define LIST_CABINET      "cabinet <number'cabinet'>"
#define LIST_ITEM         "<string'item'>"
#define LIST_ITEM_CABINET "<string'item'> in cabinet <number'cabinet'>"
