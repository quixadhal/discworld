.DT
player_shop.h
Disk World autodoc help
player_shop.h

.SH Description
.SP 5 5

Header file for player-run shops.



%^BOLD%^Introduction%^RESET%^</p> 

The aim of the player-run shop is to provide an environment where players can interact with each other, and the shop, with no interference from creators. Ideally, once the shop is set up, and with the exception of the implementation of new features or code maintenance, no further input should be required. </p> 

All administration tasks can be performed by players. These tasks typically include (but are not limited to): adding and removing stock personnel decisions (hiring/firing/disciplinary) deciding what to stock & for how much it is bought/sold increasing/decreasing stock capacity (for a price) setting pay & bonus levels. </p> 

The only involvement needed by a creator after setup is the initial role of manager until employees have been promoted to take this position. </p> 

%^BOLD%^Appearance%^RESET%^</p> 

%^BOLD%^%^CYAN%^Customers%^RESET%^%^RESET%^</p> 

From a customers' point of view, the shop should appear to function similarly to a normal shop, with the obvious addition of players acting as shopkeepers. In the absence of players, there is an npc shopkeeper to serve customers. The npc has two customer interfaces. 
.EP
.SO 6 2 -10

	*	It will recognise the standard "buy", "sell", "list", "browse" commands.</li> 
	*	It will recognise certain patterns of speech to perform the same function.</li> <\ul> The npc will then react to the customer's request, performing the same actions as a player would. </p> 

		A bell is provided on the counter to alert any employees logged onto DW in the event that the npc is not available. Employees that are clocked in are listed in the description of the shop front. It is here that a customer may make a complaint, suggestion, or apply for employment with the shop. Managers are then mailed to inform them of a new application. Confirmations of employment, or cancellations of application are also made here. </p> 

		%^BOLD%^%^CYAN%^Employees%^RESET%^%^RESET%^</p> 

		Once an application has been accepted, confirmed, and subject to any vacancies, the applicant is hired and becomes an employee of the shop. Employees are able to move past the counter into the room beyond. From here, it should be possible to access the office, storeroom and managers' office. </p> 

		Employees are responsible for replenishing stock and serving customers. Each is given a staff handbook and a special badge which acts as a talker, allowing employees to keep in touch on a dedicated channel. This channel is also used by the shop bell to alert employees to a customer in the shop. </p> 

		Before undertaking any actions in the shop, it is important that the employee clocks on in the office. Also in the office is the shop's chart, listing the lines of stock carried. This chart displays the buying and selling price, as well as the number of items currently in stock. </p> 

		The stock itself is held in the stockroom, in one or more stock cabinets. Each item is allocated a cabinet and the employee can either place items in a specific cabinet, or let the stockroom work out where they should go. Similarly, an employee can specify which cabinet to remove stock from, or let the stockroom determine that. Listing stock is an artform in itself - stock can be listed by item, cabinet, item in cabinet, or all stock. </p> 

		When buying or selling items, the employee will need to handle money. This is kept in the register behind the counter. Along with the stock, all changes made are logged and viewable by supervisors and managers. The logbook behind the counter is for employees to record for themselves what they have done and why. </p> 

		Of course, a big incentive to do this work is to be paid for it. Employees are paid every DW month, direct into their bank account. They also receive a profit-related bonus each month, which they claim from the office. The pay and bonus structure is discussed later, and ties in nicely with EchoGnomics. Along with the pay, they accumulate points towards promotion. </p> 

		Promotion is now not mandatory, and employees can deselect themselves if they wish not to be promoted. Any player with the "no score" property is automatically de- selected. Promotion is the only personnel issue that managers have no control over. It is done automatically to minimise favouritism. Once an employee has achieved sufficient points, and subject to vacancies, they are promoted to %^CYAN%^Supervisor%^CYAN%^. </p> 

		%^BOLD%^%^CYAN%^Supervisors%^RESET%^%^RESET%^</p> 

		Supervisors receive a higher wage packet than employees - 1.5 times the value, to be exact. Their duties include those of the employees, with several additions. They are responsible for deciding what items should be bought and sold, and at what price. Along with this, they assign items to cabinets. To assist them, they are able to view the general and chart logs. These logs indicate what is being bought/sold, and what updates have been made to the chart. Again, supervisors accumulate points towards promotion, and will eventually be promoted to %^CYAN%^Manager%^RESET%^. </p> 

		%^BOLD%^%^CYAN%^Managers%^RESET%^%^RESET%^</p> 

		Managers receive double the wage of employees. Their duties, however, are quite involved. They include setting staffing levels, pay rates, renting new storage space, hiring/firing/disciplinary issues, and setting the shop's policies. This is in addition to the duties performed by supervisors and employees. Delegation would seem to play a part :-) </p> 

		Managers cannot be promoted, and so have nothing to "work towards". Experience has shown, however, that players in this position gain their enthusiasm from the role itself, along with that pay packet. This view is reinforced by the fact that most managers will prefer to retire than resign. </p> 

		A retired manager has no active role in the shop, other than voting on policy suggestions, and receives no pay. Those employed by Tarnach's have been shown to be happy acting as 'mentors'. </p> 

		The main advantage of being a retired manager is the lack of commitment needed - a manager must be very active in the shop, otherwise risking demotion. This demotion follows a warning, recorded on an employee's record. The same procedure applies to supervisors_, within a longer timescale. %^CYAN%^Employees%^RESET%^have even longer, but risk being fired rather than demoted. These firings and demotions are automatic, and are one method of staff turnover to help ensure a fair system to allow new employees and promotions. If an employee has a valid reason to be away, they can notify a manager who can place them on leave. This ensures they are not fired or demoted if they are genuinely on holiday etc. </p> 

		The code also includes a hook to the refresh handler. The office is registered automatically, and appropriate action taken when an employee refreshes. 
.EO
.SO 9 2 -13

	*	If the character is deleted, the employee data is also deleted.</li> 
	*	If an applicant refreshes, their application is terminated.</li> 
	*	If a retired manager refreshes, their employment is terminated.</li> 
	*	If any other employee refreshes totally, their employment is terminated, but if they only partially refresh, they revert to being a "new employee".</li> 
.EO
.SP 5 5
</p> 

%^BOLD%^Echo-gnomics%^RESET%^</p> 

One problem with the original Tarnach's economic strategy is that employees' pay was created from thin air, whilst the profits accrued without depletion. In order to do something with the profits, they were distributed monthly between the employees by means of a bonus payment. This is still less than desirable, however, as pay is still being created from nothing. </p> 

The new system departs from this idea, and all shop's expenditure comes from shop's profits. Shop's expenditure consists of: 
.EP
.SO 12 2 -16

	*	Buying stock</li> 
	*	Monthly bonus fund</li> 
	*	Employee pay</li> 
	*	Renting store cabinets</li> 
.EO
.SP 5 5
</p> 

These are listed in order of departure from the original concept. Stock purchases have always been funded by shop's profits. The monthly bonus fund, as explained above, was added later. A new departure is that pay also comes from profits. Finally, the cabinet rental was only made possible by the redesign of the stockroom. </p> 

Another major departure from the original idea is that the npc shopkeeper now gets paid. After all, the npc does a hell of a lot of work, so why shouldn't it get paid? The npc will be paid in exactly the same way as a normal employee, except that its pay won't go to a bank account but rather 'disappear'. The npc will also claim a bonus each month, in line with a normal employee's bonus rate. This will help reduce the surplus funds, and should also give an incentive to player employees to work harder. Promotion of the npc employee probably isn't such a good idea though ;-) </p> 

Originally, the stockroom could contain an (theroetically) infinite amount of stock. This led to large save-files, and a complete loss of stock in the event of a crash (640k stock files don't tolerate crashes very well if they are writing at the time). </p> 

To combat this, stock is seperated into one or more stock cabinets, each holding a maximum of 50 items. Each store owns 5 cabinets, but more may be rented depending on the success of the shop. The rental fee is paid from the profit account. </p> 

Obviously, all this expenditure needs to be balanced by income. The only standard source of shop's income is stock sales. Other sources can be added on a shop-by-shop basis, such as the automagical pickler in Tarnach's (A-M). This results in a far more challenging role for shop's managers, having to balance income versus expenditure. Role-playing at its most business-like :-) </p>
.EP
.SP 10 5


Written by Ringo

Started 1st August 1999

.EP



.SH See also
.SP 5 5
/std/shops/player_shop/office.c, /std/shops/player_shop/mgr_office.c, /std/shops/player_shop/counter.c, /std/shops/player_shop/storeroom.c, /std/shops/player_shop/shop_front.c and /std/shops/player_shop/shopkeeper.c
.EP
.SH Defines
.SI 3
* %^BOLD%^ACCOUNTS%^RESET%^
.EI
.SP 7 5

Log entries - accounts (pay/bonus/profit)

.EP

.SI 3
* %^BOLD%^APP_ABSTAIN%^RESET%^
.EI
.SP 7 5

Applications mapping - abstentions

.EP

.SI 3
* %^BOLD%^APP_AGAINST%^RESET%^
.EI
.SP 7 5

Applications mapping - votes against

.EP

.SI 3
* %^BOLD%^APP_FOR%^RESET%^
.EI
.SP 7 5

Applications mapping - votes for

.EP

.SI 3
* %^BOLD%^APP_MAP%^RESET%^
.EI
.SP 7 5

Applications mapping

.EP

.SI 3
* %^BOLD%^APP_MESSAGE%^RESET%^
.EI
.SP 7 5

Applications mapping - application message

.EP

.SI 3
* %^BOLD%^APP_QUESTIONS%^RESET%^
.EI
.SP 7 5

Questions to ask an applicant for a job

.EP

.SI 3
* %^BOLD%^APP_TIME%^RESET%^
.EI
.SP 7 5

Applications mapping - time of application

.EP

.SI 3
* %^BOLD%^APP_TYPE%^RESET%^
.EI
.SP 7 5

Applications mapping - type of applicant

.EP

.SI 3
* %^BOLD%^APPLIED%^RESET%^
.EI
.SP 7 5

Applicant types - Just applied

.EP

.SI 3
* %^BOLD%^AWAITING%^RESET%^
.EI
.SP 7 5

Applicant types - Awaiting a vacancy

.EP

.SI 3
* %^BOLD%^BAD_BANNER%^RESET%^
.EI
.SP 7 5

Banned people mapping - banner 

.EP

.SI 3
* %^BOLD%^BAD_MAP%^RESET%^
.EI
.SP 7 5

Banned people mapping

.EP

.SI 3
* %^BOLD%^BAD_REASON%^RESET%^
.EI
.SP 7 5

Banned people mapping - reason

.EP

.SI 3
* %^BOLD%^BAD_TIME%^RESET%^
.EI
.SP 7 5

Banned people mapping - time of ban

.EP

.SI 3
* %^BOLD%^BADGE%^RESET%^
.EI
.SP 7 5

Path to the shop badge object

.EP

.SI 3
* %^BOLD%^BAN_LENGTH%^RESET%^
.EI
.SP 7 5

Length of ban from shop in days

.EP

.SI 3
* %^BOLD%^BANK_HANDLER%^RESET%^
.EI
.SP 7 5

Path to the bank handler

.EP

.SI 3
* %^BOLD%^BANKS%^RESET%^
.EI
.SP 7 5

List of banks.
The list is formatted as ({ ({ "Description", "Name" }) })
.EP
.SP 7 5
Any new banks must be added to the end of this list to avoid upsetting existing employees ;-) 
.EP

.SI 3
* %^BOLD%^BELL_TIMEOUT%^RESET%^
.EI
.SP 7 5

How long to prevent someone ringing the shop bell again (seconds)

.EP

.SI 3
* %^BOLD%^CABINET%^RESET%^
.EI
.SP 7 5

Path to the shop cabinet object

.EP

.SI 3
* %^BOLD%^CABINET_COST%^RESET%^
.EI
.SP 7 5

Price of cabinets

.EP

.SI 3
* %^BOLD%^CHART_AVE%^RESET%^
.EI
.SP 7 5

Chart mapping - average stock

.EP

.SI 3
* %^BOLD%^CHART_AVESALE%^RESET%^
.EI
.SP 7 5

Chart mapping - average daily sales

.EP

.SI 3
* %^BOLD%^CHART_BOUGHT%^RESET%^
.EI
.SP 7 5

Chart mapping - daily purchases

.EP

.SI 3
* %^BOLD%^CHART_BUY%^RESET%^
.EI
.SP 7 5

Chart mapping - buy price

.EP

.SI 3
* %^BOLD%^CHART_CAB%^RESET%^
.EI
.SP 7 5

Chart mapping - stock cabinet(s) used by this item

.EP

.SI 3
* %^BOLD%^CHART_MAX%^RESET%^
.EI
.SP 7 5

Chart mapping - maximum stock

.EP

.SI 3
* %^BOLD%^CHART_SELL%^RESET%^
.EI
.SP 7 5

Chart mapping - sell price

.EP

.SI 3
* %^BOLD%^CHART_SOLD%^RESET%^
.EI
.SP 7 5

Chart mapping - daily sales

.EP

.SI 3
* %^BOLD%^CHARTLOG%^RESET%^
.EI
.SP 7 5

Log entries - chart changes

.EP

.SI 3
* %^BOLD%^CLEAR_DELAY%^RESET%^
.EI
.SP 7 5

Call-out delay for clearing variables

.EP

.SI 3
* %^BOLD%^CLOCKED_IN%^RESET%^
.EI
.SP 7 5

Employee bit - player is clocked in.

.EP

.SI 3
* %^BOLD%^COMPLAINT_TEXT%^RESET%^
.EI
.SP 7 5

Prompt shown to player making a complaint

.EP

.SI 3
* %^BOLD%^CREATOR%^RESET%^
.EI
.SP 7 5

Default maintainer of player shops.

.EP

.SI 3
* %^BOLD%^DECLINE_LENGTH%^RESET%^
.EI
.SP 7 5

Length of time (days) before an applicant can re-apply

.EP

.SI 3
* %^BOLD%^EMP_BANK%^RESET%^
.EI
.SP 7 5

Employee mapping - bank

.EP

.SI 3
* %^BOLD%^EMP_EOM%^RESET%^
.EI
.SP 7 5

Employee mepping - = employee of month points

.EP

.SI 3
* %^BOLD%^EMP_FIRE%^RESET%^
.EI
.SP 7 5

Time (days) before employees fired for inactivity

.EP

.SI 3
* %^BOLD%^EMP_INACTIVE%^RESET%^
.EI
.SP 7 5

Employee mapping - inactivity flag

.EP

.SI 3
* %^BOLD%^EMP_MAP%^RESET%^
.EI
.SP 7 5

Employee mapping

.EP

.SI 3
* %^BOLD%^EMP_NOBONUS%^RESET%^
.EI
.SP 7 5

Employee mapping - suspended bonus

.EP

.SI 3
* %^BOLD%^EMP_NOPROMOTE%^RESET%^
.EI
.SP 7 5

Employee mapping - Skip promotion.
This is done either at employee's request, or automatically
if the "no score" property is set.
.EP
.SP 7 5
Set at -1 for "no score", or TRUE for employee request 
.EP

.SI 3
* %^BOLD%^EMP_PAY%^RESET%^
.EI
.SP 7 5

Employee mapping - this month's pay

.EP

.SI 3
* %^BOLD%^EMP_POINTS%^RESET%^
.EI
.SP 7 5

Employee mapping - points

.EP

.SI 3
* %^BOLD%^EMP_WARN%^RESET%^
.EI
.SP 7 5

Time (days) before employees warned for inactivity

.EP

.SI 3
* %^BOLD%^EMPLOYEE%^RESET%^
.EI
.SP 7 5

Employee bit - employee exists.

.EP

.SI 3
* %^BOLD%^FALSE%^RESET%^
.EI
.SP 7 5

False value (0)

.EP

.SI 3
* %^BOLD%^GENERAL%^RESET%^
.EI
.SP 7 5

Log entries - general entries

.EP

.SI 3
* %^BOLD%^HIRE_TIMEOUT%^RESET%^
.EI
.SP 7 5

Length of time (seconds) for hired applicants to confirm
their employment.

.EP

.SI 3
* %^BOLD%^HIRED%^RESET%^
.EI
.SP 7 5

Applicant types - Voted in

.EP

.SI 3
* %^BOLD%^HIST_TIMEOUT%^RESET%^
.EI
.SP 7 5

Length of time (seconds) for a player's history to expire if
there has been no update.

.EP

.SI 3
* %^BOLD%^HISTORY_HANDLER%^RESET%^
.EI
.SP 7 5

Path to the history handler used for chats through the badge

.EP

.SI 3
* %^BOLD%^last_login(N)%^RESET%^
.EI

.SI 3
* %^BOLD%^LOG_DAYS%^RESET%^
.EI
.SP 7 5

Length of time (days) to keep shop's logs

.EP

.SI 3
* %^BOLD%^LOG_ENTRY%^RESET%^
.EI
.SP 7 5

Log entries - manual entry

.EP

.SI 3
* %^BOLD%^LOG_ERROR(A,B)%^RESET%^
.EI
.SP 7 5

Log unallowed calls

.EP

.SI 3
* %^BOLD%^MANAGER%^RESET%^
.EI
.SP 7 5

Employee bit - employee is manager.

.EP

.SI 3
* %^BOLD%^MANAGER_POINTS%^RESET%^
.EI
.SP 7 5

Number of points to become a manager.

.EP

.SI 3
* %^BOLD%^MAX_BAG_ITEMS%^RESET%^
.EI
.SP 7 5

Bag maximum storage

.EP

.SI 3
* %^BOLD%^MAX_BAG_WEIGHT%^RESET%^
.EI
.SP 7 5

Bag maximum contents' weight

.EP

.SI 3
* %^BOLD%^MAX_CABINETS%^RESET%^
.EI
.SP 7 5

Maximum number of cabinets per storeroom

.EP

.SI 3
* %^BOLD%^MAX_EMP%^RESET%^
.EI
.SP 7 5

Maximum number of employees per shop

.EP

.SI 3
* %^BOLD%^MAX_LEAVE%^RESET%^
.EI
.SP 7 5

Maximum number of consecutive days an employee can be on leave

.EP

.SI 3
* %^BOLD%^MAX_STOCK%^RESET%^
.EI
.SP 7 5

Maximum number of items in stock

.EP

.SI 3
* %^BOLD%^MGR_BOOK%^RESET%^
.EI
.SP 7 5

Path to the manager's handbook

.EP

.SI 3
* %^BOLD%^MGR_DEMOTE%^RESET%^
.EI
.SP 7 5

Time (days) before managers demoted for inactivity

.EP

.SI 3
* %^BOLD%^MGR_WARN%^RESET%^
.EI
.SP 7 5

Time (days) before managers warned for inactivity

.EP

.SI 3
* %^BOLD%^MIN_CABINETS%^RESET%^
.EI
.SP 7 5

Minimum number of cabinets per storeroom

.EP

.SI 3
* %^BOLD%^MIN_EMP%^RESET%^
.EI
.SP 7 5

Minimum number of employees per shop

.EP

.SI 3
* %^BOLD%^NPC%^RESET%^
.EI
.SP 7 5

Employee bit - employee is the npc shopkeeper.

.EP

.SI 3
* %^BOLD%^NPC_DELAY%^RESET%^
.EI
.SP 7 5

Shopkeeper's movement & command delay

.EP

.SI 3
* %^BOLD%^NPC_RELOAD%^RESET%^
.EI
.SP 7 5

Shopkeeper's reload delay (seconds)

.EP

.SI 3
* %^BOLD%^PAID%^RESET%^
.EI
.SP 7 5

Paid action

.EP

.SI 3
* %^BOLD%^PERCENT_M%^RESET%^
.EI
.SP 7 5

Percentage of employees that can be managers

.EP

.SI 3
* %^BOLD%^PERCENT_S%^RESET%^
.EI
.SP 7 5

Percentage of employees that can be supervisors

.EP

.SI 3
* %^BOLD%^PERS_DELAY%^RESET%^
.EI
.SP 7 5

Call-out delay for saving personnel data

.EP

.SI 3
* %^BOLD%^PERSONNEL%^RESET%^
.EI
.SP 7 5

Log entries - personnel issues (hire, fire, promote, demote,
warnings, retire, resign, on leave etc)

.EP

.SI 3
* %^BOLD%^PLAYER_HANDLER%^RESET%^
.EI
.SP 7 5

Path to the player handler

.EP

.SI 3
* %^BOLD%^PLAYER_SHOP%^RESET%^
.EI
.SP 7 5

Path to the player-shop handler

.EP

.SI 3
* %^BOLD%^POLICY_AGAINST%^RESET%^
.EI
.SP 7 5

Policy mapping

.EP

.SI 3
* %^BOLD%^POLICY_FOR%^RESET%^
.EI
.SP 7 5

Policy mapping

.EP

.SI 3
* %^BOLD%^POLICY_MAP%^RESET%^
.EI
.SP 7 5

Policy mapping

.EP

.SI 3
* %^BOLD%^POLICY_MGR%^RESET%^
.EI
.SP 7 5

Policy mapping

.EP

.SI 3
* %^BOLD%^POLICY_TEXT%^RESET%^
.EI
.SP 7 5

Policy mapping

.EP

.SI 3
* %^BOLD%^POLICY_TIME%^RESET%^
.EI
.SP 7 5

Policy mapping

.EP

.SI 3
* %^BOLD%^POLICY_TYPE%^RESET%^
.EI
.SP 7 5

Policy type

.EP

.SI 3
* %^BOLD%^PSOBJDIR%^RESET%^
.EI
.SP 7 5

Path to player-shop objects

.EP

.SI 3
* %^BOLD%^PURCHASE%^RESET%^
.EI
.SP 7 5

Log entries - items purchased by shop.
Items added to stock, cash removed from till

.EP

.SI 3
* %^BOLD%^SALE%^RESET%^
.EI
.SP 7 5

Log entries - items sold by shop.
Items removed from stock, cash added to till

.EP

.SI 3
* %^BOLD%^SAVE_DELAY%^RESET%^
.EI
.SP 7 5

Call-out delay for saving employee & shop data

.EP

.SI 3
* %^BOLD%^SHOP_BAG%^RESET%^
.EI
.SP 7 5

Path to the shopping bag

.EP

.SI 3
* %^BOLD%^SHOP_CHART%^RESET%^
.EI
.SP 7 5

Path to the chart

.EP

.SI 3
* %^BOLD%^SHOP_NOTICE%^RESET%^
.EI
.SP 7 5

Path to the policy notice

.EP

.SI 3
* %^BOLD%^SPR_DEMOTE%^RESET%^
.EI
.SP 7 5

Time (days) before supervisors demoted for inactivity

.EP

.SI 3
* %^BOLD%^SPR_WARN%^RESET%^
.EI
.SP 7 5

Time (days) before supervisors warned for inactivity

.EP

.SI 3
* %^BOLD%^STAFF_BOOK%^RESET%^
.EI
.SP 7 5

Path to staff handbook

.EP

.SI 3
* %^BOLD%^START_PAY%^RESET%^
.EI
.SP 7 5

Default pay amount

.EP

.SI 3
* %^BOLD%^STOCK_PER_CABINET%^RESET%^
.EI
.SP 7 5

Maximum number of items per cabinet

.EP

.SI 3
* %^BOLD%^SUGGEST_TEXT%^RESET%^
.EI
.SP 7 5

Prompt shown to player making a suggestion

.EP

.SI 3
* %^BOLD%^SUPER_POINTS%^RESET%^
.EI
.SP 7 5

Number of points to become a supervisor.

.EP

.SI 3
* %^BOLD%^SUPERVISOR%^RESET%^
.EI
.SP 7 5

Employee bit - employee is supervisor.

.EP

.SI 3
* %^BOLD%^test_cre(N)%^RESET%^
.EI

.SI 3
* %^BOLD%^test_player(N)%^RESET%^
.EI

.SI 3
* %^BOLD%^test_prop(N,P)%^RESET%^
.EI

.SI 3
* %^BOLD%^TRUE%^RESET%^
.EI
.SP 7 5

True value (1)

.EP

.SI 3
* %^BOLD%^UNPAID%^RESET%^
.EI
.SP 7 5

Unpaid action

.EP

.SI 3
* %^BOLD%^UNSET_STR%^RESET%^
.EI
.SP 7 5

Initial value for some strings

.EP

.SI 3
* %^BOLD%^VABSTAIN%^RESET%^
.EI
.SP 7 5

Abstain on applicant

.EP

.SI 3
* %^BOLD%^VAGAINST%^RESET%^
.EI
.SP 7 5

Vote against an applicant

.EP

.SI 3
* %^BOLD%^VFOR%^RESET%^
.EI
.SP 7 5

Vote for an applicant

.EP

.SI 3
* %^BOLD%^VOTE_TIMEOUT%^RESET%^
.EI
.SP 7 5

Length of time (seconds) before votes are considered final

.EP

