/**
 * Header file for player-run shops.
 * 
 * <p><b>Introduction</b></p>
 * <p>The aim of the player-run shop is to provide an environment where
 * players can interact with each other, and the shop, with no interference
 * from creators. Ideally, once the shop is set up, and with the exception
 * of the implementation of new features or code maintenance, no further
 * input should be required.
 * </p>
 * <p>All administration tasks can be performed by players. These tasks
 * typically include (but are not limited to): adding and removing stock
 * personnel decisions (hiring/firing/disciplinary) deciding what to stock
 * & for how much it is bought/sold increasing/decreasing stock capacity
 * (for a price) setting pay & bonus levels.
 * </p>
 * <p>The only involvement needed by a creator after setup is the initial
 * role of manager until employees have been promoted to take this position. 
 * </p>
 * 
 * <p><b>Appearance</b></p>
 * <p><b><i>Customers</i></b></p>
 * <p>From a customers' point of view, the shop should appear to function
 * similarly to a normal shop, with the obvious addition of players acting
 * as shopkeepers. In the absence of players, there is an npc shopkeeper
 * to serve customers. The npc has two customer interfaces.
 * <ul>
 * <li>It will recognise the standard "buy", "sell", "list",
 * "browse" commands.</li>
 * <li>It will recognise certain patterns of speech to perform
 * the same function.</li>
 * <\ul>
 * The npc will then react to the customer's request, performing the same
 * actions as a player would.
 * </p>
 * <p>A bell is provided on the counter to alert any employees logged
 * onto DW in the event that the npc is not available. Employees that
 * are clocked in are listed in the description of the shop front. It
 * is here that a customer may make a complaint, suggestion, or apply
 * for employment with the shop. Managers are then mailed to inform
 * them of a new application. Confirmations of employment, or
 * cancellations of application are also made here.
 * </p>
 * 
 * <p><b><i>Employees</i></b></p>
 * <p>Once an application has been accepted, confirmed, and subject to
 * any vacancies, the applicant is hired and becomes an employee of the
 * shop. Employees are able to move past the counter into the room beyond.
 * From here, it should be possible to access the office, storeroom and
 * managers' office.
 * </p>
 * <p>Employees are responsible for replenishing stock and serving
 * customers. Each is given a staff handbook and a special badge which
 * acts as a talker, allowing employees to keep in touch on a dedicated 
 * channel. This channel is also used by the shop bell to alert employees
 * to a customer in the shop.
 * </p>
 * <p>Before undertaking any actions in the shop, it is important that the
 * employee clocks on in the office. Also in the office is the shop's chart,
 * listing the lines of stock carried. This chart displays the buying and
 * selling price, as well as the number of items currently in stock.
 * </p>
 * <p>The stock itself is held in the stockroom, in one or more stock
 * cabinets. Each item is allocated a cabinet and the employee can either
 * place items in a specific cabinet, or let the stockroom work out where
 * they should go. Similarly, an employee can specify which cabinet to
 * remove stock from, or let the stockroom determine that. Listing stock
 * is an artform in itself - stock can be listed by item, cabinet, item in
 * cabinet, or all stock.
 * </p>
 * <p>When buying or selling items, the employee will need to handle
 * money. This is kept in the register behind the counter. Along with
 * the stock, all changes made are logged and viewable by supervisors
 * and managers. The logbook behind the counter is for employees to
 * record for themselves what they have done and why.
 * </p>
 * <p>Of course, a big incentive to do this work is to be paid for it.
 * Employees are paid every DW month, direct into their bank account.
 * They also receive a profit-related bonus each month, which they claim
 * from the office. The pay and bonus structure is discussed later, and
 * ties in nicely with EchoGnomics. Along with the pay, they accumulate
 * points towards promotion.
 * </p>
 * <p>Promotion is now not mandatory, and employees can deselect
 * themselves if they wish not to be promoted. Any player with the
 * "no score" property is automatically de- selected. Promotion is the
 * only personnel issue that managers have no control over. It is done
 * automatically to minimise favouritism. Once an employee has achieved
 * sufficient points, and subject to vacancies, they are promoted to 
 * <i>Supervisor<i>.
 * </p>
 * 
 * <p><b><i>Supervisors</i></b></p>
 * <p>Supervisors receive a higher wage packet than employees - 1.5
 * times the value, to be exact. Their duties include those of the
 * employees, with several additions. They are responsible for deciding
 * what items should be bought and sold, and at what price. Along with
 * this, they assign items to cabinets. To assist them, they are able to
 * view the general and chart logs. These logs indicate what is being
 * bought/sold, and what updates have been made to the chart. Again,
 * supervisors accumulate points towards promotion, and will eventually
 * be promoted to <i>Manager</i>.
 * </p> 
 * 
 * <p><b><i>Managers</i></b></p>
 * <p>Managers receive double the wage of employees. Their duties,
 * however, are quite involved. They include setting staffing levels,
 * pay rates, renting new storage space, hiring/firing/disciplinary
 * issues, and setting the shop's policies. This is in addition to
 * the duties performed by supervisors and employees.
 * Delegation would seem to play a part :-) 
 * </p>
 * <p>Managers cannot be promoted, and so have nothing to "work towards".
 * Experience has shown, however, that players in this position gain their
 * enthusiasm from the role itself, along with that pay packet. This view
 * is reinforced by the fact that most managers will prefer to retire than
 * resign.
 * </p>
 * <p>A retired manager has no active role in the shop, other than voting
 * on policy suggestions, and receives no pay. Those employed by Tarnach's
 * have been shown to be happy acting as 'mentors'. 
 * </p>
 * <p>The main advantage of being a retired manager is the lack of
 * commitment needed - a manager must be very active in the shop,
 * otherwise risking demotion. This demotion follows a warning, 
 * recorded on an employee's record. The same procedure applies to
 * supervisors_, within a longer timescale. <i>Employees</i> have even
 * longer, but risk being fired rather than demoted. These firings and
 * demotions are automatic, and are one method of staff turnover to help
 * ensure a fair system to allow new employees and promotions. If an
 * employee has a valid reason to be away, they can notify a manager
 * who can place them on leave. This ensures they are not fired or
 * demoted if they are genuinely on holiday etc.
 * </p>
 * <p>The code also includes a hook to the refresh handler. The office
 * is registered automatically, and appropriate action taken when an 
 * employee refreshes.
 * <ul>
 * <li>If the character is deleted, the employee data is also deleted.</li>
 * <li>If an applicant refreshes, their application is terminated.</li>
 * <li>If a retired manager refreshes, their employment is terminated.</li>
 * <li>If any other employee refreshes totally, their employment is
 * terminated, but if they only partially refresh, they revert to being
 * a "new employee".</li>
 * </ul>
 * </p>
 * 
 * <p><b>Echo-gnomics</b></p>
 * <p>One problem with the original Tarnach's economic strategy is that
 * employees' pay was created from thin air, whilst the profits accrued
 * without depletion. In order to do something with the profits, they
 * were distributed monthly between the employees by means of a bonus
 * payment. This is still less than desirable, however, as pay is still
 * being created from nothing. 
 * </p>
 * <p>The new system departs from this idea, and all shop's expenditure
 * comes from shop's profits. Shop's expenditure consists of:
 * <ul>
 * <li>Buying stock</li>
 * <li>Monthly bonus fund</li>
 * <li>Employee pay</li>
 * <li>Renting store cabinets</li>
 * </ul></p> 
 * <p>These are listed in order of departure from the original concept.
 * Stock purchases have always been funded by shop's profits. The monthly
 * bonus fund, as explained above, was added later. A new departure is
 * that pay also comes from profits. Finally, the cabinet rental was
 * only made possible by the redesign of the stockroom. 
 * </p>
 * <p>Another major departure from the original idea is that the npc
 * shopkeeper now gets paid. After all, the npc does a hell of a lot
 * of work, so why shouldn't it get paid? The npc will be paid in exactly
 * the same way as a normal employee, except that its pay won't go to a
 * bank account but rather 'disappear'. The npc will also claim a bonus
 * each month, in line with a normal employee's bonus rate. This will help
 * reduce the surplus funds, and should also give an incentive to player
 * employees to work harder. Promotion of the npc employee probably isn't
 * such a good idea though ;-) 
 * </p>
 * <p>Originally, the stockroom could contain an (theroetically) infinite
 * amount of stock. This led to large save-files, and a complete loss of
 * stock in the event of a crash (640k stock files don't tolerate crashes
 * very well if they are writing at the time). 
 * </p>
 * <p>To combat this, stock is seperated into one or more stock cabinets,
 * each holding a maximum of 50 items. Each store owns 5 cabinets, but
 * more may be rented depending on the success of the shop. The rental
 * fee is paid from the profit account. 
 * </p>
 * <p>Obviously, all this expenditure needs to be balanced by income.
 * The only standard source of shop's income is stock sales. Other
 * sources can be added on a shop-by-shop basis, such as the automagical
 * pickler in Tarnach's (A-M). This results in a far more challenging
 * role for shop's managers, having to balance income versus expenditure.
 * Role-playing at its most business-like :-) 
 * </p>
 * @see /std/shops/player_shop/office.c
 * @see /std/shops/player_shop/mgr_office.c
 * @see /std/shops/player_shop/counter.c
 * @see /std/shops/player_shop/storeroom.c
 * @see /std/shops/player_shop/shop_front.c
 * @see /std/shops/player_shop/shopkeeper.c
 * @author Ringo
 * @started 1st August 1999
 */

 /**
 * Default maintainer of player shops.
 */
#define CREATOR "calroth"


/**
 * Initial value for some strings
 */
#define UNSET_STR "__RINGO_WOZ_ERE__"

/**
 * Path to player-shop objects
 */
#define PSOBJDIR "/obj/misc/player_shop/"
/**
 * Path to the shop cabinet object
 */
#define CABINET (PSOBJDIR + "shop_cabinet")
/**
 * Path to the shop badge object
 */
#define BADGE (PSOBJDIR + "shop_badge")
/**
 * Path to staff handbook
 */
#define STAFF_BOOK (PSOBJDIR + "staff_book")
/**
 * Path to the manager's handbook
 */
#define MGR_BOOK (PSOBJDIR + "mgr_book")
/**
 * Path to the shopping bag
 */
#define SHOP_BAG (PSOBJDIR + "shopping_bag")
/**
 * Path to the chart
 */
#define SHOP_CHART (PSOBJDIR + "shop_chart")
/**
 * Path to the policy notice
 */
#define SHOP_NOTICE (PSOBJDIR + "shop_notice")

/**
 * True value (1)
 */
#define TRUE 1
/**
 * False value (0)
 */
#define FALSE 0

/**
 * Paid action
 */
#define PAID 1
/**
 * Unpaid action
 */
#define UNPAID 0

/**
 * Employee mapping - points
 */
#define EMP_POINTS 0
/**
 * Employee mapping - bank
 */
#define EMP_BANK 1
/**
 * Employee mapping - this month's pay
 */
#define EMP_PAY 2
/**
 * Employee mapping - inactivity flag
 */
#define EMP_INACTIVE 3
/**
 * Employee mapping - suspended bonus
 */
#define EMP_NOBONUS 4
/**
 * Employee mapping - Skip promotion.
 * This is done either at employee's request, or automatically
 * if the "no score" property is set.<br>
 * Set at -1 for "no score", or TRUE for employee request
 */
#define EMP_NOPROMOTE 5
/**
 * Employee mepping - = employee of month points
 */
#define EMP_EOM 6

/**
 * Employee bit - employee exists.
 */
#define EMPLOYEE 1
/**
 * Employee bit - employee is the npc shopkeeper.
 */
#define NPC 2
/**
 * Employee bit - employee is supervisor.
 */
#define SUPERVISOR 4
/**
 * Employee bit - employee is manager.
 */
#define MANAGER 8
/**
 * Employee bit - player is clocked in.
 */
#define CLOCKED_IN 16

/**
 * Employee mapping
 */
#define EMP_MAP ({EMPLOYEE,0,0,0,1,0,0,})

/**
 * Number of points to become a manager.
 */
#define MANAGER_POINTS 1000
/**
 * Number of points to become a supervisor.
 */
#define SUPER_POINTS 500

/**
 * Banned people mapping - reason
 */
#define BAD_REASON 0
/**
 * Banned people mapping - banner 
 */
#define BAD_BANNER 1
/**
 * Banned people mapping - time of ban
 */
#define BAD_TIME 2

/**
 * Banned people mapping
 */
#define BAD_MAP ({"","",0,})

/**
 * Applications mapping - type of applicant
 */
#define APP_TYPE 0
/**
 * Applications mapping - time of application
 */
#define APP_TIME 1
/**
 * Applications mapping - votes for
 */
#define APP_FOR 2
/**
 * Applications mapping - votes against
 */
#define APP_AGAINST 3
/**
 * Applications mapping - abstentions
 */
#define APP_ABSTAIN 4
/**
 * Applications mapping - application message
 */
#define APP_MESSAGE 5

/**
 * Applicant types - Just applied
 */
#define APPLIED 1
/**
 * Applicant types - Voted in
 */
#define HIRED 2
/**
 * Applicant types - Awaiting a vacancy
 */
#define AWAITING 3

/**
 * Vote for an applicant
 */
#define VFOR      1
/**
 * Vote against an applicant
 */
#define VAGAINST  0
/**
 * Abstain on applicant
 */
#define VABSTAIN -1

/**
 * Applications mapping
 */
#define APP_MAP ({APPLIED,0,({}),({}),({}),"",})

/**
 * Minimum number of cabinets per storeroom
 */
#define MIN_CABINETS 5
/**
 * Maximum number of cabinets per storeroom
 */
#define MAX_CABINETS 20
/**
 * Maximum number of items per cabinet
 */
#define STOCK_PER_CABINET 50
/**
 * Maximum number of items in stock
 */
#define MAX_STOCK (MAX_CABINETS * STOCK_PER_CABINET)
/**
 * Price of cabinets
 */
#define CABINET_COST 400000

/**
 * Log entries - manual entry
 */
#define LOG_ENTRY 1
/**
 * Log entries - items purchased by shop.
 * Items added to stock, cash removed from till
 */
#define PURCHASE 2
/**
 * Log entries - items sold by shop.
 * Items removed from stock, cash added to till
 */
#define SALE 3
/**
 * Log entries - personnel issues (hire, fire, promote, demote,
 * warnings, retire, resign, on leave etc)
 */
#define PERSONNEL 4
/**
 * Log entries - accounts (pay/bonus/profit)
 */
#define ACCOUNTS 5
/** 
 * Log entries - general entries
 */
#define GENERAL 6
/**
 * Log entries - chart changes
 */
#define CHARTLOG 7

/**
 * Chart mapping - maximum stock
 */
#define CHART_MAX 0
/**
 * Chart mapping - average stock
 */
#define CHART_AVE 1
/**
 * Chart mapping - buy price
 */
#define CHART_BUY 2
/**
 * Chart mapping - sell price
 */
#define CHART_SELL 3
/**
 * Chart mapping - stock cabinet(s) used by this item
 */
#define CHART_CAB 4
/**
 * Chart mapping - average daily sales
 */
#define CHART_AVESALE 5
/**
 * Chart mapping - daily sales
 */
#define CHART_SOLD 6
/**
 * Chart mapping - daily purchases
 */
#define CHART_BOUGHT 7

/** 
 * Policy mapping
 */
#define POLICY_TIME 0
/** 
 * Policy mapping
 */
#define POLICY_MGR 1
/** 
 * Policy mapping
 */
#define POLICY_TEXT 2
/** 
 * Policy mapping
 */
#define POLICY_FOR 3
/** 
 * Policy mapping
 */
#define POLICY_AGAINST 4
/**
 * Policy type
 */
#define POLICY_TYPE 5

/** 
 * Policy mapping
 */
#define POLICY_MAP ({0,"","",({}),({}),0,})

/**
 * Path to the player-shop handler
 */
#define PLAYER_SHOP "/obj/handlers/player_shop"
/**
 * Path to the history handler used for chats through the badge
 */
#define HISTORY_HANDLER "/obj/handlers/hist_handler"
/**
 * Path to the bank handler
 */
#define BANK_HANDLER "/obj/handlers/bank_handler"
/**
 * Path to the player handler
 */
#define PLAYER_HANDLER "/obj/handlers/player_handler"

/**
 * List of banks.
 * The list is formatted as ({ ({ "Description", "Name" }) })<br>
 * Any new banks must be added to the end of this list to avoid 
 * upsetting existing employees ;-)
 */
#define BANKS ({ ({ "Bing's First National", "Bing's First", }), \
                 ({ "Lancrastian Farmers' Co-operative", "LFC", }), \
                 ({ "Klatchian Continental", "Klatchian Continental", }), \
              }) 

/**
 * How long to prevent someone ringing the shop bell again (seconds)
 */
#define BELL_TIMEOUT 120

/**
 * Minimum number of employees per shop
 */
#define MIN_EMP 10
/**
 * Maximum number of employees per shop
 */
#define MAX_EMP 40
/**
 * Percentage of employees that can be managers
 */
#define PERCENT_M 10
/**
 * Percentage of employees that can be supervisors
 */
#define PERCENT_S 20

/**
 * Default pay amount
 */
#define START_PAY 20

/** 
 * Maximum number of consecutive days an employee can be on leave
 */
#define MAX_LEAVE 14
/**
 * Length of ban from shop in days
 */
#define BAN_LENGTH 28
/**
 * Length of time (days) before an applicant can re-apply
 */
#define DECLINE_LENGTH 14
/**
 * Time (days) before managers demoted for inactivity
 */
#define MGR_DEMOTE 14
/**
 * Time (days) before managers warned for inactivity
 */
#define MGR_WARN 7
/**
 * Time (days) before supervisors demoted for inactivity
 */
#define SPR_DEMOTE 28
/**
 * Time (days) before supervisors warned for inactivity
 */
#define SPR_WARN 14
/**
 * Time (days) before employees fired for inactivity
 */
#define EMP_FIRE 62
/**
 * Time (days) before employees warned for inactivity
 */
#define EMP_WARN 14


/**
 * Bag maximum storage
 */
#define MAX_BAG_ITEMS 10
/**
 * Bag maximum contents' weight
 */
#define MAX_BAG_WEIGHT 50

/**
 * Length of time (days) to keep shop's logs
 */
#define LOG_DAYS 14

/**
 * Length of time (seconds) for hired applicants to confirm
 * their employment.
 */
#define HIRE_TIMEOUT (86400*14)

/**
 * Length of time (seconds) for a player's history to expire if
 * there has been no update.
 */
#define HIST_TIMEOUT (86400*91)

/**
 * Length of time (seconds) before votes are considered final
 */
#define VOTE_TIMEOUT (86400*14)

/**
 * Shopkeeper's movement & command delay
 */
#define NPC_DELAY (2 + random(3))
/**
 * Shopkeeper's reload delay (seconds)
 */
#define NPC_RELOAD 3600

/**
 * Call-out delay for saving employee & shop data
 */
#define SAVE_DELAY 1
/**
 * Call-out delay for saving personnel data
 */
#define PERS_DELAY 5
/**
 * Call-out delay for clearing variables
 */
#define CLEAR_DELAY 300

/**
 * Prompt shown to player making a complaint
 */
#define COMPLAINT_TEXT "Please fully state your complaint below.  " \
                       "Include as much detail as you can and any " \
                       "evidence/witnesses to support your complaint."

/**
 * Prompt shown to player making a suggestion
 */
#define SUGGEST_TEXT "Please detail your suggestion below."

/**
 * Questions to ask an applicant for a job
 */
#define APP_QUESTIONS ({ "Why do you want to work for "+ \
                     _office->query_shop_name()+ "?", \
                     "What is your (DW) age?", \
                     "What Guild do you belong to?", \
                     "What relevant experience do you have of collecting " \
                      " and/or recognising our stock?", \
                     "How much time (hours/week) do you think you can " \
                     "commit to the shop?  Please also tell us at what " \
                     "time you will usually be on.", \
                     "Do you already know anyone who is an employee " \
                     "here, who could recommend you?  If so, who?", \
                     "Have you previously held a position at this shop?  " \
                     "If so, why did you leave?", \
                     "Do you currently work at any other shop?", \
                     "Have you applied for employment here before?  " \
                     "If so, what was the outcome?", \
                     "Do you have anything else you'd like to tell us in " \
                     "support of your application?  Please expand on above " \
                     "points if necessary.  What you say here will influence " \
                     "whether or not you are hired." \
                })

/**
 * Log unallowed calls
 */
#define LOG_ERROR(A,B) log_file( "/log/PLAYER_SHOP", "%s: %s : %s called by %s\n",\
                       ctime(time()), A, B, previous_object()->query_name() )

#define last_login(N) (PLAYER_HANDLER)->test_last(N)
#define test_cre(N) (PLAYER_HANDLER)->test_creator(N)
#define test_player(N) (PLAYER_HANDLER)->test_user(N)
#define test_prop(N,P) (PLAYER_HANDLER)->test_property(N,P)
