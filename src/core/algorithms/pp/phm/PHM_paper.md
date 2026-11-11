PHM: Mining Periodic High-Utility Itemsets
Philippe Fournier-Viger1, Jerry Chun-Wei Lin2,
Quang-Huy Duong3, Thu-Lan Dam3,4,
1 School of Natural Sciences and Humanities, Harbin Institute of Technology
Shenzhen Graduate School, China
2 School of Computer Science and Technology, Harbin Institute of Technology
Shenzhen Graduate School, China
3 College of Computer Science and Electronic Engineering, Hunan University, China
4 Faculty of Information Technology, Hanoi University of Industry, Vietnam
philfv@hitsz.edu.cn, jerrylin@ieee.org,
huydqyb@gmail.com, lanfict@gmail.com
Abstract. High-utility itemset mining is the task of discovering high-
utility itemsets, i.e. sets of items that yield a high profit in a customer
transaction database. High-utility itemsets are useful, as they provide
informationaboutprofitablesetsofitemsboughtbycustomerstoretail
store managers, which can then use this information to take strategic
marketing decisions. An inherent limitation of traditional high-utility
itemset mining algorithms is that they are inappropriate to discover re-
curring customer purchase behavior, although such behavior is common
in real-life situations (for example, a customer may buy some products
every day, week or month). In this paper, we address this limitation by
proposing the task of periodic high-utility itemset mining. The goal is
to discover groups of items that are periodically bought by customers
and generate a high profit. An efficient algorithm named PHM (Peri-
odic High-utility itemset Miner) is proposed to efficiently enumerate all
periodic high-utility itemsets. Experimental results show that the PHM
algorithm is efficient, and can filter a huge amount of non periodic pat-
terns to reveal only the desired periodic high-utility itemsets.
Keywords: high-utility itemset, periodic itemset, average periodicity
1 Introduction
High-utilityitemsetmining(HUIM)[4,5,8–10,13]isapopulardataminingtask.
It has attracted a lot of attention in recent years. It extends the traditional
problem of Frequent Itemset Mining (FIM) [1]. This latter consists of discov-
ering frequent itemsets. i.e. groups of items (itemsets) appearing frequently in
a transaction database [1]. FIM has many applications. However, an important
limitationofFIMisthatitassumesthateachitemcannotappearmorethanonce
in each transaction and that all items have the same importance (e.g. weight,
unit profit or value). High-Utility Itemset Mining (HUIM) addresses this issue
byconsideringthateachitemmayhavenonbinarypurchasequantitiesintrans-
actions and that each item has a weight (e.g. unit profit). The goal of HUIM

is to discover itemsets having a high utility (e.g. yielding a high profit) in a
transaction database. Besides, market basket analysis, HUIM has several other
applicationssuchaswebsiteclickstreamanalysis,andbiomedicalapplications[9,
13]. Mining high-utility itemsets is widely recognized as more challenging than
FIM because the utility measure used in HUIM is not anti-monotonic, i.e. a
high utility itemset may have supersets or subsets having lower, equal or higher
utilities [4]. Thus, techniques for reducing the search space in FIM cannot be
directly reused in HUIM.
Though several algorithms have been proposed for HUIM [4,5,8–10,13], an
inherent limitation of these algorithms is that they are inappropriate to dis-
cover recurring customer purchase behavior, although such behavior is common
in real-life situations. For example, in a retail store, some customers may buy
some set of products on approximately a daily or weekly basis. Detecting these
purchase patterns is useful to better understand the behavior of customers and
thus adapt marketing strategies, for example by offering specific promotions to
cross-promote products such as reward or points to customers who are buying a
set of products periodically. In the field of FIM, algorithms have been proposed
to discover periodic frequent patterns (PFP) [2,3,6,11,7,12] in a transaction
database. However, these algorithms are inadequate to find periodic patterns
that yield a high profit, as they only select patterns based on their frequency.
Hence, these algorithms may find a huge amount of periodic patterns that gen-
erate a low profit and miss many rare periodic patterns that yield a high profit.
To address this limitation of previous work, this paper proposes the task of
periodic high-utility itemset mining. The goal is to efficently discover all groups
of items that are bought together periodically and generate a high profit, in
a customer transaction database. The contributions of this paper are fourfold.
First, the concept of periodic patterns used in FIM is combined with the con-
ceptofHUIstodefineanewtypeofpatternsnamedperiodichigh-utilityitemsets
(PHIs),anditspropertiesarestudied.Second,novelmeasuresofapattern’spe-
riodicity named average periodicity and minimum periodicity are introduced to
provide a flexible way of assessing the periodicity of patterns. Third, an effi-
cient algorithm named PHM (Periodic High-utility itemset Miner) is proposed
to efficiently discover the periodic high-utility itemsets. Fourth, an extensive
experimental evaluation is carried to compare the efficiency of PHM with the
state-of-the-art FHM algorithm for HUIM. Experimental results show that the
PHMalgorithmisefficient,andcanfilterahugeamountofnonperiodicpatterns
to reveal only the desired itemsets.
The rest of this paper is organized as follows. Section 2, 3, 4, 5 and 6 respec-
tivelypresentspreliminariesrelatedtoHUIM,relatedwork,thePHMalgorithm,
the experimental evaluation and the conclusion.
2 Related work
This section reviews related work in high-utility itemset mining and periodic
frequent pattern mining.

| 2.1 | High-utility |     | itemset | mining |     |     |     |     |
| --- | ------------ | --- | ------- | ------ | --- | --- | --- | --- |
Definition 1 (transaction database). Let I be a set of items (symbols). A
transaction database is a set of transactions D = {T 1 ,T 2 ,...,T n } such that for
each transaction T , T ∈I and T has a unique identifier c called its Tid. Each
|     |     |     | c c |     | c   |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
item i ∈ I is associated with a positive number p(i), called its external utility
(e.g. unit profit). For each transaction T c such that i ∈ T c , a positive number
| q(i,T | ) is | called the | internal | utility | of i (e.g. | purchase | quantity). |     |
| ----- | ---- | ---------- | -------- | ------- | ---------- | -------- | ---------- | --- |
c
Example 1. Consider the database of Fig. 1, which will be used as running ex-
ample. This database contains seven transactions (T ,T ...T ). Transaction T
|     |     |     |     |     |     |     | 1 2 | 7 3 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
indicates that items a, b, c, d, and e appear in this transaction with an internal
utility of respectively 1, 5, 1, 3 and 1. Fig. 2 indicates that the external utility
| of  | these           | items are        | respectively |          | 5, 2, 1, 2 | and 3. |             |                |
| --- | --------------- | ---------------- | ------------ | -------- | ---------- | ------ | ----------- | -------------- |
|     | Table           | 1: A transaction |              | database |            | Table  | 2: External | utility values |
|     | TID Transaction |                  |              |          |            |        | Item        | a b c d e      |
|     | T (a,1),(c,1),  |                  |              |          |            |        | Unit profit | 52123          |
1
T (e,1)
2
T (a,1),(b,5),(c,1),(d,3),(e,1)
3
T 4 (b,4),(c,3),(d,3),(e,1)
T 5 (a,1),(c,1),(d,1)
T (a,2),(c,6),(e,2)
6
T (b,2),(c,2),(e,1)
7
Definition 2 (utility of an item/itemset). The utility of an item i in a
transaction T is denoted as u(i,T ) and defined as p(i)×q(i,T ). The utility
|     |     | c   |     |     | c   |     |     | c   |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
of an itemset X (a group of items X ⊆ I) in a transaction T c is denoted as
(cid:80)
u(X,T ) and defined as u(X,T ) = u(i,T ). The utility of an itemset X
|     | c   |     |     |     | c i∈X |     | c   |     |
| --- | --- | --- | --- | --- | ----- | --- | --- | --- |
(in a database) is denoted as u(X) and defined as u(X) = (cid:80) u(X,T ),
Tc∈g(X) c
| where | g(X) | is the | set of | transactions | containing |     | X.  |     |
| ----- | ---- | ------ | ------ | ------------ | ---------- | --- | --- | --- |
Example 2. The utility of item a in T 6 is u(a,T 6 ) = 5×2 = 10. The utility of
| theitemset{a,c}inT |     |     | isu({a,c},T |     | )=u(a,T |     | )+u(c,T )=5×2+1×6=16. |     |
| ------------------ | --- | --- | ----------- | --- | ------- | --- | --------------------- | --- |
|                    |     |     | 6           |     | 6       | 6   | 6                     |     |
The utility of the itemset {a,c} (in the database) is u({a,c}) = u(a)+u(c) =
u(a,T )+u(a,T )+u(a,T )+u(a,T )+u(c,T )+u(c,T )+u(c,T )+u(c,T )=
|     | 1   | 3   |     | 5   | 6   | 1   | 3   | 5 6 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
5+5+5+10+1+1+1+6=34.
Definition 3 (high-utility itemset mining). The problem of high-utility
itemset mining is to discover all high-utility itemsets [4,5,8–10,13]. An itemset
X is a high-utility itemset if its utility u(X) is no less than a user-specified
| minimum |     | utility threshold |     | minutil | given | by the | user. |     |
| ------- | --- | ----------------- | --- | ------- | ----- | ------ | ----- | --- |

Example 3. Ifminutil=30,thecompletesetofHUIsis{a,c}:34,{a,c,e}:31,
{b,c,d}:34,{b,c,d,e}:40,{b,c,e}:37,{b,d}:30,{b,d,e}:36,and{b,e}:31,
| where each | HUI is annotated |     | with | its utility. |     |     |     |     |
| ---------- | ---------------- | --- | ---- | ------------ | --- | --- | --- | --- |
InHUIM,theutilitymeasureisnotmonotonicoranti-monotonic[10,13],i.e.,
anitemsetmayhaveautilitylower,equalorhigherthantheutilityofitssubsets.
Several HUIM algorithms circumvent this problem by overestimating the utility
ofitemsetsusingtheTransaction-Weighted Utilization (TWU) measure[10,13],
| which is | anti-monotonic, | and | defined | as follows. |     |     |     |     |
| -------- | --------------- | --- | ------- | ----------- | --- | --- | --- | --- |
Definition 4 (Transaction weighted utilization). The transaction utility
(TU) of a transaction T c is the sum of the utility of all the items in T c . i.e.
(cid:80)
TU(T ) = u(x,T ). The transaction-weighted utilization (TWU) of an
| c   | x∈Tc | c   |     |     |     |     |     |     |
| --- | ---- | --- | --- | --- | --- | --- | --- | --- |
itemset X is defined as the sum of the transaction utility of transactions con-
(cid:80)
| taining | X, i.e. TWU(X)= |     |         | TU(T ). |     |     |     |     |
| ------- | --------------- | --- | ------- | ------- | --- | --- | --- | --- |
|         |                 |     | Tc∈g(X) | c       |     |     |     |     |
Example 4. The TUs of T ,T ,T ,T , T , T and T are respectively 6,3, 25, 20,
|     |     | 1   | 2 3 | 4 5 6 | 7   |     |     |     |
| --- | --- | --- | --- | ----- | --- | --- | --- | --- |
8, 22 and 9. The TWU of single items a, b, c, d, e are respectively 61, 54, 90, 53
| and 79.            | TWU({c,d})=TU(T                        |      | )+TU(T | )+TU(T        | )=25+20+8=53. |              |              |     |
| ------------------ | -------------------------------------- | ---- | ------ | ------------- | ------------- | ------------ | ------------ | --- |
|                    |                                        |      | 3      | 4             | 5             |              |              |     |
| Theorem            | 1 (PruningsearchspaceusingtheTWU).LetX |      |        |               |               |              | beanitemset, |     |
| if TWU(X)<minutil, |                                        | then | X and  | its supersets | are           | low utility. | [10]         |     |
AlgorithmssuchasTwo-Phase[10],BAHUI[8],PB[5],andUPGrowth+[13]
utilizestheabovepropertytoprunethesearchspace.Theyoperateintwophases.
In the first phase, they identify candidate high utility itemsets by calculating
their TWUs. In the second phase, they scan the database to calculate the exact
utilityofallcandidatesfoundinthefirstphasetoeliminatelowutilityitemsets.
Recently, an alternative algorithm called HUI-Miner[9] was proposed to mine
HUIs directly using a single phase. Then, a faster depth-first search algorithm
FHM [4] was proposed, which extends HUI-Miner. In FHM, each itemset is
associatedwithastructurenamedutility-list [4,9].Utility-listsallowcalculating
the utility of an itemset quickly by making join operations with utility-lists of
| shorter | patterns. Utility-lists |     | are | defined as follows. |     |     |     |     |
| ------- | ----------------------- | --- | --- | ------------------- | --- | --- | --- | --- |
Definition 5 (Utility-list). Let (cid:31) be any total order on items from I. The
utility-list of an itemset X in a database D is a set of tuples such that there is a
tuple (tid,iutil,rutil) for each transaction T containing X. The iutil element
tid
of a tuple is the utility of X in T tid . i.e., u(X,T tid ). The rutil element of a tuple
(cid:80)
| is defined | as                    |     | u(i,T | ).  |     |     |     |     |
| ---------- | --------------------- | --- | ----- | --- | --- | --- | --- | --- |
|            | i∈Ttid∧i(cid:31)x∀x∈X |     |       | tid |     |     |     |     |
Example 5. Assume that (cid:31) is the alphabetical order. The utility-list of {a} is
{(T 1 ,5,1), T 3 ,5,20), (T 5 ,5,3), (T 6 ,10,12)}. The utility-list of {d} is {(T 3 ,6,3),
| (T ,6,3),   | (T ,2,0)}. | The utility-list |          | of {a,d} | is {(T ,11,3), | (T   | ,7,0)}.   |          |
| ----------- | ---------- | ---------------- | -------- | -------- | -------------- | ---- | --------- | -------- |
| 4           | 5          |                  |          |          | 3              |      | 5         |          |
| To discover | HUIs,      | FHM              | performs | a single | database       | scan | to create | utility- |
lists of patterns containing single items. Then, longer patterns are obtained
by performing the join operation of utility-lists of shorter patterns. The join

operation for single items is performed as follows. Consider two items x,y such
that x (cid:31) y, and their utility-lists ul({x}) and ul({y}). The utility-list of {x,y}
isobtainedbycreatingatuple(ex.tid,ex.iutil+ey.iutil,ey.rutil)foreachpairs
of tuples ex ∈ ul({x}) and ey ∈ ul({y}) such that ex.tid = ey.tid. The join
operation for two itemsets P ∪{x} and P ∪{y} such that x (cid:31) y is performed
as follows. Let ul(P), ul({x}) and ul({y}) be the utility-lists of P, {x} and {y}.
The utility-list of P ∪{x,y} is obtained by creating a tuple (ex.tid,ex.iutil+
ey.iutil −ep.iutil,ey.rutil) for each set of tuples ex ∈ ul({x}), ey ∈ ul({y}),
ep ∈ ul(P) such that ex.tid = ey.tid = ep.tid. Calculating the utility of an
itemset using its utility-list and pruning the search space is done as follows.
Property 1 (Calculating utility of an itemset using its utility-list). The utility of
an itemset is the sum of iutil values in its utility-list [9].
Theorem 2 (Pruning search space using utility-lists). Let X be an item-
set. Let the extensions of X be the itemsets that can be obtained by appending
an item y to X such that y (cid:31)i, ∀i∈X. If the sum of iutil and rutil values in
ul(X) is less than minutil, X and its extensions are low utility [9].
FHM is very efficient. However, an important limitation of current HUIM
algorithms is that they are not designed for discovering periodic patterns.
2.2 Periodic Frequent Pattern Mining
InthefieldofFIM,algorithmshavebeenproposedtodiscoverperiodicalfrequent
patterns(PFP)[2,3,6,11,7,12]inatransactiondatabase.DiscoveringPFPhas
applications in many domains such as web mining, bioinformatics, and market
basket analysis [12]. The concept of PFP is defined as follows [12].
Definition 6 (Periods of an itemset). LettherebeadatabaseD ={T ,T ,
1 2
...,T } containing n transactions, and an itemset X. The set of transactions
n
containing X is denoted as g(X) = {T ,T ...,T }, where 1 ≤ g < g <
g1 g2 gk 1 2
... < g ≤ n. Two transactions T ⊃ X and T ⊃ X are said to be consecutive
k x y
with respect to X if there does not exist a transaction T ∈ g(X) such that
w
x < w < y. The period of two consecutive transactions T and T in g(X) is
x y
defined as pe(T ,T ) = (y − x), that is the number of transactions between
x y
T and T . The periods of an itemset X is a list of periods defined as ps(X) =
x y
{g −g ,g −g ,g −g ,...g −g ,g −g },whereg andg +1areconstants
1 0 2 1 3 2 k k−1 k+1 k 0 k
(cid:83)
defined as g =0 and g +1=n. Thus, ps(X)= (g −g ).
0 k 1≤z≤k+1 z z−1
Example 6. For the itemset {a,c}, The list of transactions containing {a,c} is
g({a,c}) = {T ,T ,T ,T }. Thus, the periods of this itemset are ps({a,c}) =
1 3 5 6
{1,2,2,1,1}.
Definition 7 (Periodic Frequent Pattern). The maximum periodicity of
anitemsetX isdefinedasmaxper(X)=max(ps(X))[12].AnitemsetX isape-
riodic frequent pattern (PFP) if |g(X)| ≥ minsup and maxper(X) < maxPer,
where minsup and maxPer are user-defined thresholds [12].

ThefirstalgorithmforminingPFPsisPFP-Tree[12].Itutilizesatree-based
and pattern-growth approach for discovering PFPs. Then, the MTKPP algo-
rithm [2] was proposed for discovering the k most frequent PFPs in a database,
where k is a user-specified parameter. MTKPP utilizes a vertical structure to
maintaininformationaboutitemsetsinthedatabaseAvariationofthePF-Tree
algorithm named the ITL-Tree was also introduced [3] to reduce the time for
mining PFPs by approximating the periodicity of itemsets. Another approxi-
mate algorithm for PFP mining was recently proposed [7]. Other extensions of
the PF-Tree algorithm named MIS-PF-tree [6] and MaxCPF [11] were respec-
tively proposed to mine PFPs using multiple minsup thresholds, and multiple
| minsup | and minper |            | thresholds. |                |     |            |     |            |         |
| ------ | ---------- | ---------- | ----------- | -------------- | --- | ---------- | --- | ---------- | ------- |
| An     | important  | limitation |             | of traditional |     | algorithms | for | PFP mining | is that |
they are inadequate to find periodic patterns that yield a high profit, since they
only consider the support (frequency) of patterns. Hence, they may find a huge
amountofperiodicpatternsthatyieldalowprofitandmissmanyrareperiodical
| patterns | that | yield a   | high profit. |     |     |     |     |     |     |
| -------- | ---- | --------- | ------------ | --- | --- | --- | --- | --- | --- |
| 3 The    | PHM  | algorithm |              |     |     |     |     |     |     |
To address the aforementioned limitation of HUI and PFP mining algorithms,
thissectionintroducestheconceptofperiodichigh-utilityitemsets(PHUIs).The
first subsection present novel measures to assess the periodicity of HUIs, while
the second subsection presents and efficient algorithm named PHM (Periodic
| High-Utility  | Itemset |     | Miner)      | to discover | PHUIs           | efficiently. |          |     |     |
| ------------- | ------- | --- | ----------- | ----------- | --------------- | ------------ | -------- | --- | --- |
| 3.1 Measuring |         | the | periodicity |             | of high-utility |              | patterns |     |     |
AdrawbackofthemaximumperiodicitymeasureusedbymostPFPalgorithms
is that an itemset is automatically discarded if it has a single period of length
greater than the maxPer threshold. Thus, this measure may be viewed as too
strict. To provide a more flexible way of evaluating the periodicity of patterns,
| the concept | of  | average | periodicity | is  | introduced | in  | the proposed | algorithm. |     |
| ----------- | --- | ------- | ----------- | --- | ---------- | --- | ------------ | ---------- | --- |
Definition 8 (Averageperiodicityofanitemset). Theaverageperiodicity
| of an itemset | X   | is defined | as  | avgper(X)= |     | (cid:80) | /|ps(X)|. |     |     |
| ------------- | --- | ---------- | --- | ---------- | --- | -------- | --------- | --- | --- |
g∈ps(X)
Example 7. The periods of itemsets {a,c} and {e} are respectively ps({a,c})=
{1,2,2,1,1} and ps({e}) = {2,1,1,2,1,0}. The average periodicities of these
| itemsets | are respectively |     | avgper({a,c})=1.4 |     |     | and | avgper({e})=1.16. |     |     |
| -------- | ---------------- | --- | ----------------- | --- | --- | --- | ----------------- | --- | --- |
Lemma 1 (Relationshipbetweenaverageperiodicityandsupport).Let
X be an itemset appearing in a database D. An alternative and equivalent way
| of calculating |     | the average | periodicity |     | of X | is avgper(X)=|D|/(|g(X)|+1). |     |     |     |
| -------------- | --- | ----------- | ----------- | --- | ---- | ---------------------------- | --- | --- | --- |
Proof. Let g(X) = {T g1 ,T g2 ,...,T gk } be the set of transactions containing X,
(cid:80)
such that g < g < ... < g . By definition, avgper(X) = /|ps(X)|.
|     | 1   | 2   |     | k   |     |     |     | g∈ps(X) |     |
| --- | --- | --- | --- | --- | --- | --- | --- | ------- | --- |

(cid:80)
To prove that the lemma holds, we need to show that /|ps(X)| =
g∈ps(X)
|D|/(|g(X)|+1).
(cid:80)
(1) We first show that =|D|, as follows:
g∈ps(X)
(cid:80)
=(g −g )+(g −g )+...(g −g )+(g −g )}
g∈ps(X) 1 0 2 1 k k−1 k+1 k
(cid:80)
= =g +(g −g )+(g −g )+...(g −g )+(g )
g∈ps(X) 0 1 1 2 2 k k k+1
=g −g = |D|.
k+1 0
(2) We then show that |ps(X)|=|g(X)|+1, as follows:
(cid:83)
Bydefinition,ps(X)= (g −g ).Thus,thesetps(X)containsk+1
1≤z≤k+1 z z−1
elements. Since X appears in k transactions, sup(X) = k, and thus |ps(X)| =
|g(X)|+1.
Since (1) and (2) holds, the lemma holds. (cid:116)(cid:117)
The above lemma is important as it provides an efficient way of calculating
the average periodicity of itemsets in a database D. The term |D| can be cal-
culated once, and thereafter the average periodicity of any itemset X can be
obtained by only calculating |g(X)|+1, and then dividing |D| by the result.
ThisismoreefficientthancalculatingtheaverageperiodicityusingDefinition8.
Besides,thislemmaisimportantasitshowsthatthereisarelationshipbetween
the support used in FIM and the average periodicity of a pattern.
Although the average periodicity is useful as it measures what is the typi-
cal period length of an itemset, it should not be used as the sole measure for
evaluating the periodicity of a pattern because it does not consider whether an
itemset has periods that vary widely or not. For example, the itemset {b,d} has
anaverageperiodicityof2.33.However,thisismisleadingsincethisitemsetonly
appears in transaction T and T , and its periods ps({T ,T }) = {3,1,4} vary
3 4 3 4
widely.Intuitively,thispatternshouldnotbeaperiodicpattern.Toavoidfinding
patterns havingperiods thatvarywidely, oursolution is to combine theaverage
periodicity measure with other periodicity measure(s). The following measures
are combined with the average periodicity to achieve this goal.
First, we define the minimum periodicity of an itemset as minper(X) =
min(ps(X) to avoid discovering itemsets having some very short periods. But
this measure is not reliable since the first and last period of an itemset are
respectively equal to 1 or 0 if the itemset respectively appears in the first or
the last transaction of the database. For example, the last period of itemset
{e} is 0, because it appears in the last transaction (T ), and thus its minimum
7
periodicity is 0. Our solution to this issue is to exclude the first and last periods
of an itemset from the calculation of the minimum periodicity. Moreover, if the
set of periods is empty as a result of this exclusion, the minimum periodicity is
defined as ∞. In the rest of this paper, we consider this definition.
Second, we consider the maximum periodicity of an itemset maxper(X) as
defined in the previous section. The rationale for using this measure in combi-
nation with the average periodicity is that it can avoids discovering periodical
patterns that do not occur for long periods of time.
In terms of calculation cost, a reason for choosing the minimum periodicity,
maximum periodicity and average periodicity as measure is that they can be
calculated very efficiently for an itemset X by scanning the list of transactions

g(X) only once. That is, calculating these measures do not require to store the
set of periods ps(X) in memory. Conversely, other measure such as the stan-
dard deviation would require to calculate all periods of an itemset beforehand.
Thus, we define the concept of periodic high-utility itemsets by considering the
minimum periodicity, maximum periodicity and average periodicity measures.
Definition 9 (Periodic High-Utility Itemsets). Let minutil, minAvg,
maxAvg, minPer and maxPer be positive numbers, provided by the user. An
itemsetX isaperiodichigh-utilityitemset ifandonlyifminAvg ≤avgper(X)≤
maxAvg, minper(X)≥minPer, maxper(X)≤maxPer, and u(X)≥minutil.
| For        | example, if minutil | =20,        | minPer    | =1,        | maxPer      | =3,       | minAvg          | =1, and |
| ---------- | ------------------- | ----------- | --------- | ---------- | ----------- | --------- | --------------- | ------- |
| maxAvg     | =2, the complete    | set         | of PHUIs  | is         | shown in    | table     | 3.              |         |
|            | Table               | 3: The set  | of PHUIs  | in         | the running | example   |                 |         |
|            | Itemset             | u(X) |g(X)| | minper(X) |            | maxper(X)   | avgper(X) |                 |         |
|            | {b}                 | 22 3        | 1         |            | 3           | 1.75      |                 |         |
|            | {b,e}               | 31 3        | 1         |            | 3           | 1.75      |                 |         |
|            | {b,c,e}             | 37 3        | 1         |            | 3           | 1.75      |                 |         |
|            | {b,c}               | 28 3        | 1         |            | 3           | 1.75      |                 |         |
|            | {a}                 | 25 4        | 1         |            | 2           | 1.4       |                 |         |
|            | {a,c}               | 34 4        | 1         |            | 2           | 1.4       |                 |         |
|            | {c,e}               | 27 4        | 1         |            | 3           | 1.4       |                 |         |
| To develop | an efficient        | algorithms  |           | for mining | PHUIs,      |           | it is important | to de-  |
signefficientpruningstrategies.Tousetheperiodicitymeasuresforpruningthe
| search space, | the following | theorems |     | are presented. |     |     |     |     |
| ------------- | ------------- | -------- | --- | -------------- | --- | --- | --- | --- |
Lemma 2 (Monotonicity of the average periodicity). Let X and Y be
| itemsets | such that X | ⊂Y. It | follows | that avgper(Y)≥avgper(X). |     |     |     |     |
| -------- | ----------- | ------ | ------- | ------------------------- | --- | --- | --- | --- |
Proof. The average periodicities of X and Y are respectively avgper(X) =
|D|/(|g(X)|+1) and avgper(Y) = |D|/(|g(Y)|+1). Because X ⊂ Y, it fol-
lows that g(Y)⊆g(X). Hence, avgper(Y)≥avgper(X). (cid:116)(cid:117)
Lemma 3 (Monotonicity of the minimum periodicity). LetX andY be
| itemsets | such that X | ⊂Y. It | follows | that minper(Y)≥minper(X). |     |     |     |     |
| -------- | ----------- | ------ | ------- | ------------------------- | --- | --- | --- | --- |
Proof. Since X ⊂ Y, g(Y) ⊆ g(X). If g(Y) = g(X), then X and Y have the
same periods, and thus minper(Y) = minper(X). If g(Y) ⊂ g(X), then for
each transaction T ∈ g(X) \ g(Y), the corresponding periods in ps(X) will
x
be replaced by a larger period in ps(Y). Thus, any period in ps(Y) cannot be
smaller than a period in ps(X). Hence, minper(Y)≥minper(X). (cid:116)(cid:117)
Lemma 4 (Monotonicity of the maximum periodicity). Let X and Y
| be itemsets | such that | X ⊂Y. | It follows | that | maxper(Y)≥maxper(X) |     |     | [12]. |
| ----------- | --------- | ----- | ---------- | ---- | ------------------- | --- | --- | ----- |

Theorem 3 (Maximum periodicity pruning). Let X be an itemset ap-
pearing in a database D. X and its supersets are not PHUIs if maxper(X) >
maxPer. Thus, if this condition is met, the search space consisting of X and all
its supersets can be discarded.
Proof. By definition, if maxper(X)>maxPer, X is not a PHUI. By Lemma 4,
supersets of X are also not PHUIs.
Theorem 4 (Averageperiodicitypruning). LetX beanitemsetappearing
in a database D. X is not a PHUI as well as all of its supersets if avgper(X)>
maxAvg, or equivalently if |g(X)|<(|D|/maxAvg)−1. Thus, if this condition
is met, the search space consisting of X and all its supersets can be discarded.
Proof. By definition, if avgper(X)>maxAvg, X is not a PHUI. By Lemma 2,
supersetsofX arealsonotPHUIs.Thepruningconditionavgper(X)>maxAvg
isrewrittenas:|D|/(|g(X)|+1)>maxAvg.Thus,1/(|g(X)|+1)>maxAvg/|D|,
which can be further rewritten as |g(X)|+1 < |D|/maxAvg, and as |g(X)| <
(|D|/maxAvg)−1. (cid:116)(cid:117)
3.2 The algorithm
The proposed PHM algorithm is a utility-list based algorithm, inspired by the
FHM algorithm [4], where the utility-list of each itemset X is annotated with
two additional values: minper(X) and maxper(X). The main procedure of
PHM (Algorithm 1) takes a transaction database as input, and the minutil,
minAvg, maxAvg, minPer and maxPer thresholds. The algorithm first scans
thedatabasetocalculateTWU({i}),minper({i}),maxper({i}),and|g({i})|for
eachitemi∈I.Then,thealgorithmcalculatesthevalueγ =(|D|/maxAvg)−1
tobelaterusedforpruningitemsetsusingTheorem4.Then,thealgorithmiden-
tifies the set I∗ of all items having a TWU no less than minutil, a maximum
periodicity no greater than maxPer, and appearing in no less than γ transac-
tions (other items are ignored since they cannot be part of a PHUI by Theorem
1, 3 and 4). The TWU values of items are then used to establish a total order
(cid:31) on items, which is the order of ascending TWU values (as suggested in [9]).
A database scan is then performed. During this database scan, items in trans-
actions are reordered according to the total order (cid:31), the utility-list of each item
i ∈ I∗ is built and a structure named EUCS (Estimated Utility Co-Occurrence
Structure) is built [4]. This latter structure is defined as a set of triples of the
form (a,b,c) ∈ I∗ ×I∗ ×R. A triple (a,b,c) indicates that TWU({a,b}) = c.
The EUCS can be implemented as a triangular matrix (as shown in Fig. 1 for
the running example), or as a hashmap of hashmaps where only tuples of the
form (a,b,c) such that c(cid:54)=0 are kept. After the construction of the EUCS, the
depth-first search exploration of itemsets starts by calling the recursive proce-
dure Search with the empty itemset ∅, the set of single items I∗, γ, minutil,
minAvg, minPer, maxPer, the EUCS structure, and |D|.
TheSearchprocedure(Algorithm2)takesasinputanitemsetP,extensions
ofP havingtheformPzmeaningthatPzwaspreviouslyobtainedbyappending

|     | TID  |     | Transactions     |     |     |   Item    | a   | b  c  | d   | e  f  | g   |     |     |     |
| --- | ---- | --- | ---------------- | --- | --- | --------- | --- | ----- | --- | ----- | --- | --- | --- | --- |
|     | T    |     | (a,1)(c,1)(d,1)  |     |     |   Profit  | 5   | 2  1  | 2   | 3  1  | 1   |     |     |     |
1
|     | T  2 |                                   | (a,2)(c,6)(e,2)(g,5)  |     |     |     |     |     |     |     |     |     |     |     |
| --- | ---- | --------------------------------- | --------------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|     | T    | (a,1)(b,2)(c,1)(d,6),(e,1),(f,5)  |                       |     |     |     |     |     |     |     |     |     |     |     |
3
|     | T   |     | (b,4)(c,3)(d,3)(e,1)  |     |     |     |     |     |     |     |     |     |     |     |
| --- | --- | --- | --------------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
4
|     | T   |     | (b,2)(c,2)(e,1)(g,2)  |     |     |     |     |     |     |     |     |     |     |     |
| --- | --- | --- | --------------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
5

TU(T_1) = 7   TU(T_2) = 27     TU(T_3) =  30    TU(T_4) = 20    TU(T_5) = 11

|     |     |     |     |     |     | TID  |     | Transactions  |     |     |   Item  | a  b  c  | d  e  | f  g  |
| --- | --- | --- | --- | --- | --- | ---- | --- | ------------- | --- | --- | ------- | -------- | ----- | ----- |
TWU(a) = 65    TWU(b)= 61  TWU(c)=  T 96          (a,1)(c,1)(d,1)  TW   U( P d ro )= fi 5 t  8  5       TW 2  U( 1 e   )= 2 8   8  3      1  1
1
TWU(f)= 30   TWU(g) = 38  T 2   (a,2)(c,6)(e,2)(g,5)
|     |     |     |     |     |     |     | T  (a,1)(b,2)(c,1)(d,6),(e,1),(f,5)  |     |     |     |     |       |     |     |
| --- | --- | --- | --- | --- | --- | --- | ------------------------------------ | --- | --- | --- | --- | ----- | --- | --- |
3
|     |     |     |     |     |     |     | T   | (b,4)(c,3)(d,3)(e,1)  |     |     |     |       |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --------------------- | --- | --- | --- | ----- | --- | --- |
4
|     |     |     |     |     |     |     | T   | (b,2)(c,2)(e,1)(g,2)  |     |     |     |       |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --------------------- | --- | --- | --- | ----- | --- | --- |
5

|     | TID  | TU  |   Item  | TWU    |     | Item  | a  b  | c   | d   | e   | f   |     |     |     |
| --- | ---- | --- | ------- | ------ | --- | ----- | ----- | --- | --- | --- | --- | --- | --- | --- |
|     | T    | 8   |         | a  65  |     | b     | 30    |     |     |     |     |     |     |     |
1 TU(T_1) = 7   TU(T_2) = 27     TU(T_3) =  30    TU(T_4) = 20    TU(T_5) = 11
|     | T   | 27  |     | b  61  |     | c   | 65  61  |     |     |     |     |     |     |     |
| --- | --- | --- | --- | ------ | --- | --- | ------- | --- | --- | --- | --- | --- | --- | --- |
2
|     | T   | 30  |     | c  96  |     | d   | 38  50  | 58  |     |     |     |     |     |     |
| --- | --- | --- | --- | ------ | --- | --- | ------- | --- | --- | --- | --- | --- | --- | --- |
3
T   20    d  58    TeW  U(5a7)  = 6615    7 T7 W5U0( b) = 61   TWU(c)= 96
|                  |         | 4       |             |              |        |          |                                        |     |                |      |            |       |       | TWU(d)=58    TWU(e)=88    |
| ---------------- | ------- | ------- | ----------- | ------------ | ------ | -------- | -------------------------------------- | --- | -------------- | ---- | ---------- | ----- | ----- | ------------------------- |
|                  | T       | 5 11    |             | e  88        |        | f        | 30  30                                 | 30  | 30             | 30   |            |       |       |                           |
|                  |         |         |             |              |        | TgW      | U(2f7)=   3308    T3W8 U0( g) =3 83 8  |     |                |      |            |       |       |                           |
|                  |         |         |             | f  30        |        |          |                                        |     |                |      | 0          |       |       |                           |
|                  |         |         |             | g  38        |        |          |                                        |     |                |      |            |       |       |                           |
| A lgorithm       |         | 1:      | The PHM     | algorithm    |        |          |                                        |     |                |      |            |       |       |                           |
|                  | input   | : D: a  | transaction | database,    |        |          |                                        |     |                |      |            |       |       |                           |
| support minutil, |         |         | minAvg,     | maxAvg,      | minPer |          | and maxPer:                            |     | the thresholds |      |            |       |       |                           |
|                  |         |         |             |              |        |          | TID                                    | TU  |   Item         | TWU  |   Item  a  | b  c  | d  e  | f                         |
|                  | output: | the set | of periodic | high-utility |        | itemsets |                                        |     |                |      |            |       |       |                           |
|                  |         |         |             |              |        |          | T                                      | 8   |   a            | 65   |   b  30    |       |       |                           |
1
|     | Scan D   | o nce   | to ca l cula | te TW U({i}), | minper({i}T) |     | , max2p7e r({ i}), |     | abn d | |g({6i}1) | |          |         |     |     |
| --- | -------- | ------- | ------------ | ------------- | ------------ | --- | ------------------ | --- | ----- | ----------- | -------- | ------- | --- | --- |
| 1   | I tem    |   a     | b            | c   d         |              |     | 2                  |     |       |             |   c  65  | 61      |     |     |
|     | for ea c | h ite m | i∈ I;        |               |              |     |                    |     |       |             |          |         |     |     |
|     | b        |   1     |              |               |              |     | T                  | 30  |   c   | 96          |   d  38  | 50  58  |     |     |
3
2 γ ← (| D |/ m a x A vg ) − 1 ; T   2 0     d   5 8     e   5 7   6 1   7 7   5 0
|     | I∗ c |   4 |   3   |     |     |     | 4   |     |     |     |     |     |     |     |
| --- | ---- | --- | ----- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
3 ← e a c h it e m i s u c h t h at T WU(i)≥minutil, T |g (   {i}) 1 | 1 ≥   γ a   nd e  8 8     f  3 0   3 0   3 0   3 0   30
|     | d   |   2 |   2   | 3     |     |     | 5   |     |     |     |     |     |     |     |
| --- | --- | --- | ----- | ----- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
maxper({i})≤maxPer;
|     | e               | 2        | 3                | 4  2      |           |           |      |       |   f      | 30  |   g  27  | 38  38  | 0  38  | 0   |
| --- | --------------- | -------- | ---------------- | --------- | --------- | --------- | ---- | ----- | -------- | --- | -------- | ------- | ------ | --- |
| 4   | Let (cid:31) be | the      | tota l orde      | r of T WU | ascending | value     | s on | I∗ ;  |          |     |          |         |        |     |
|     |                 |          |                  |           |           |           |      |       |   g      | 38  |          |         |        |     |
|     | Scan D          | to build | the utility-list | of        | each      | item i∈I∗ | and  | build | the EUCS |     |          |         |        |     |
5

structure;
|     | TW     | I∗, |             |         |         |          |         |       |       |     |     |     |     |     |
| --- | ------ | --- | ----------- | ------- | ------- | -------- | ------- | ----- | ----- | --- | --- | --- | --- | --- |
| 6   | Search | (∅, | γ, minutil, | minAvg, | minPer, |          | maxPer, | EUCS, | |D|); |     |     |     |     |     |
|     | U      |     |             |         |         | support  |         |       |       |     |     |     |     |     |

|     |     | a   | b   | c  d  |     |     | Item  | a  b  | c   | d   |     |     |     |     |
| --- | --- | --- | --- | ----- | --- | --- | ----- | ----- | --- | --- | --- | --- | --- | --- |
Item
|     | b   | 25     |          |         |     |     | b        | 1     |      |     |     |     |     |     |
| --- | --- | ------ | -------- | ------- | --- | --- | -------- | ----- | ---- | --- | --- | --- | --- | --- |
|     | c   | 61     | 54       |         |     |     | c        | 4  3  |      |     |     |     |     |     |
|     | d   | 33     | 45       | 53      |     |     | d        | 2  2  | 3    |     |     |     |     |     |
|     | e   | 47     | 54       | 76  45  |     |     | e        | 2  3  | 4    | 2   |     |     |     |     |
|     |     |        |          |         |     |     |          |       |      |     |     |     |     |     |
|     |     | Fig.1: | The EUCS |         |     |     |   Fig.2: | The   | ESCS |     |     |     |     |     |
TW
U

anitemz toP,γ,minutil,minAvg,minPer,max Per,theEUCS,and|D|.The
search procedure performs a loop on each extenItseimon  Pax  ofbP . Inc thisdl oop, the
averageperiodicityofPxisobtainedbydividingb|D  |by25t he numb erof  elements
in the utility list of Px plus one (by Lemma 1). Tch en,6if1 the54a ver age p eriodicity
of Px is in the [minAvg,maxAvg] interval, theds um o3f3 the45i uti5l3v alu es of the
utility-listofPxisnolessthanminutil(cf.Properet y1),4t7h em54in im7u6m  /m45a ximum
periodicity of Px is no less/not greater  than minPer/maxPer according to
the values stored in its utility-list, then Px is a PHUI and it is output. Then,
if the sum of iutil and rutil values in the utility-list of Px are no less than
minutil, the number of elements in the utility list of Px is no less than γ,
and maxper(Px) is no greater than maxPer, it means that extensions of Px
should be explored (by Theorem 1, 3 and 4). This is performed by merging Px
with all extensions Py of P such that y (cid:31) x to form extensions of the form
Pxy containing |Px|+1 items. The utility-list of Pxy is then constructed by
calling the Construct procedure (cf. Algorithm 3), to join the utility-lists of
P, Px and Py. This latter procedure is mainly the same as in HUI-Miner [9],
with the exception that periods are calculated during utility-list construction to
obtain maxPer(Pxy) and minPer(Pxy) (not shown). Then, a recursive call to
the Search procedure with Pxy is done to calculate its utility and explore its
extension(s).TheSearchprocedurestartsfromsingleitems,recursivelyexplores
the search space of itemsets by appending single items, and only prunes the

search space using Theorem 1, 3 and 4. Thus, it can be easily seen that this
procedure is correct and complete to discover all PHUIs.
Algorithm 2: The Search procedure
input : P: an itemset, ExtensionsOfP: a set of extensions of P, γ, minutil,
minAvg, minPer, maxPer, the EUCS structure, |D|
output: the set of periodic high-utility itemsets
1 foreach itemset Px∈ExtensionsOfP do
2 avgperPx←|D|/(|Px.utilitylist|+1);
3 if SUM(Pxy.utilitylist.iutils) ≥minutil∧
minAvg≤avgperPx≤maxAvg∧Px.utilitylist.minp≥
minPer∧Px.utilitylist.maxp≤maxPer∧ then output Px;
4 if SUM(Px.utilitylist.iutils)+SUM(Px.utilitylist.rutils)≥minutil ∧
avgperPx≥γ and Px.utilitylist.maxp≤maxPer then
5 ExtensionsOfPx←∅;
6 foreach itemset Py∈ExtensionsOfP such that y(cid:31)x do
7 if ∃(x,y,c)∈ EUCS such that c≥minutil) then
8 Pxy←Px∪Py;
9 Pxy.utilitylist← Construct (P,Px,Py);
10 ExtensionsOfPx←ExtensionsOfPx ∪ {Pxy};
11 end
12 end
13 Search (Px, ExtensionsOfPx, γ, minutil, minAvg, minPer, maxPer,
EUCS, |D|);
14 end
15 end
Furthermore, in the implementation of PHM, two additional optimizations
are included, which are briefly described next.
Optimization 1. Estimated Average Periodicity Pruning (EAPP).
The PHM algorithms creates a structure called EUCS to store the TWU of all
pairs of items occurring in the database, and this structure is used to prune any
itemsetPxy containingapairofitems{x,y}havingaTWUlowerthanminutil
(Line 7 of the search procedure). The strategy EAPP is a novel strategy that
uses the same idea but prune itemsets using the average periodicity instead of
theutility.Duringtheseconddatabasescan,anovelstructurecalledESCS(Es-
timatedSupportCo-occurrenceStructure)iscreatedtostore|g({x,y})|foreach
pair of items {x,y} (as shown in Figure 2). Then, Line 7 of the search procedure
is modified to prune itemset Pxy if |g({x,y})| is less than γ by Theorem 4.
Optimization 2. Abandoning List Construction early (ALC). An-
other strategy introduced in PHM is to stop constructing the utility-list of an
itemset if a specific condition is met, indicating that the itemset cannot be a
PHUI. By Theorem 4, an itemset Pxy cannot be a PHUI, if it appears in less
thanγ =(|D|/maxAvg)−1transactions.ThestrategyALCconsistsofmodify-

| Algorithm | 3:               | The Construct |           | procedure |           |                    |
| --------- | ---------------- | ------------- | --------- | --------- | --------- | ------------------ |
| input     | : P: an          | itemset,      | Px: the   | extension | of P with | an item x, Py: the |
|           | extension        | of            | P with an | item      | y         |                    |
| output:   | the utility-list |               | of Pxy    |           |           |                    |
UtilityListOfPxy←∅;
1
| foreach | tuple | ex∈Px.utilitylist |     | do  |     |     |
| ------- | ----- | ----------------- | --- | --- | --- | --- |
2
| 3 if | ∃ey∈Py.utilitylist                               |         | and             | ex.tid=exy.tid | then      |                |
| ---- | ------------------------------------------------ | ------- | --------------- | -------------- | --------- | -------------- |
| 4    | if P.utilitylist(cid:54)=∅                       |         | then            |                |           |                |
| 5    | Search                                           | element | e∈P.utilitylist |                | such that | e.tid=ex.tid.; |
| 6    | exy←(ex.tid,ex.iutil+ey.iutil−e.iutil,ey.rutil); |         |                 |                |           |                |
| 7    | end                                              |         |                 |                |           |                |
| 8    | else                                             |         |                 |                |           |                |
| 9    | exy←(ex.tid,ex.iutil+ey.iutil,ey.rutil);         |         |                 |                |           |                |
| 10   | end                                              |         |                 |                |           |                |
period ←calculatePeriod(exy.tid,UtilityListOfPxy);
| 11  | exy                                        |     |     |     |     |        |
| --- | ------------------------------------------ | --- | --- | --- | --- | ------ |
| 12  | UpdateMinPerMaxPer(UtilityListOfPxy,period |     |     |     |     | exy ); |
| 13  | UtilityListOfPxy←UtilityListOfPxy∪{exy};   |     |     |     |     |        |
14 end
end
15
| 16 return | UtilityListPxy; |     |     |     |     |     |
| --------- | --------------- | --- | --- | --- | --- | --- |
ing the Construct procedure (Algorithm 3) as follows. The first modification is
to initialize a variable max with the value γ in Line 1. The second modification
is to the following lines, where the utility-list of Pxy is constructed by checking
if each tuple in the utility-lists of Px appears in the utility-list of Py (Line 3).
For each tuple not appearing in Py, the variable max is decremented by 1. If
maxissmallerthanγ,theconstructionoftheutility-listofPxy canbestopped
because|g(Pxy)|willnotbehigherthanγ.ThusPxyisnotaPHUIbyTheorem
| 4, and its     | extensions | can | also be | ignored. |     |     |
| -------------- | ---------- | --- | ------- | -------- | --- | --- |
| 4 Experimental |            |     | Study   |          |     |     |
We performed an experimental study to assess the performance of PHM. The
experiment was performed on a computer with a sixth generation 64 bit Core i5
processorrunningWindows10,andequippedwith12GBoffreeRAM.Wecom-
paredtheperformanceoftheproposedPHMalgorithmwiththestate-of-the-art
FHM algorithm for mining HUIs. All memory measurements were done using
the Java API. The experiment was carried on four real-life datasets commonly
used in the HUIM litterature: retail, mushroom, chainstore and foodmart. These
datasets have varied characteristics and represents the main types of data typi-
callyencounteredinreal-lifescenarios(dense,sparseandlongtransactions).Let
|I|, |D| and A represents the number of transactions, distinct items and average
transaction length of a dataset. retail is a sparse dataset with many different
items(|I|=16,470,|D|=88,162,A=10,30).mushroom isadensedatasetwith

long transactions (|I| = 119, |D| = 8,124, A = 23). chainstore is a dataset that
contains a huge number of transactions (|I| = 461, |D| = 1,112,949, A = 7.23).
foodmart is a sparse dataset (|I| = 1,559, |D| = 4,141, A = 4.4). The chainstore
and foodmart datasets are real-life customer transaction databases containing
real external and internal utility values. The retail and mushroom datasets con-
tains synthetic utility values, generated randomly [9,13]. The source code of all
algorithms and datasets can be downloaded from http://goo.gl/Y6eBdz.
In the experiment, PHM was run on each dataset with fixed minper and
minAvg values, while varying the minutil threshold and the values of the
maxAvg and maxper parameters. In these experiments, the values for the pe-
riodicity thresholds have been found empirically for each dataset (as they are
dataset specific), and were chosen to show the trade-off between the number of
periodic patterns found and the execution time. Note that results for varying
the minper and minAvg values are not shown because these parameters have
less influence on the patterns found than the other parameters. Thereafter, the
notation PHM V-W-X-Y represents the PHM algorithm with minper = V,
maxper =W, minAvg =X, and maxAVG=Y.
Fig. 3 compares the execution times of PHM for various parameter values
and FHM. Fig. 4, compares the number of PHUIs found by PHM for various
parameter values, and the number of HUIs found by FHM.
Retail Mushroom
100 FHM 1000 FHM
80 P PH HM M 1 1 - - 1 5 0 00 00 0- -5 5- -5 25 00 0 PHM 1-1000-5-500
)s(
em
60 PHM 1-250-5-100 )s(
em
100 PHM 1-500-5-250
itnuR
40
itnuR
10
20
1
0 10000 20000 30000 40000 50000 0 2000000 4000000 6000000 8000000 10000000
minutil minutil
Chaintstore Foodmart
1000 10 000
FHM
PHM 1-5000-5-500
PHM 1-2000-5-200 1 000
)s(
em
itnuR
1
1
0
0
0
)sm
( em
itnuR
1
1
0
0
0
F
P
H
H
M
M 10-5000-5-500
PHM 1-2000-5-200
PHM 10-1000-5-100
1 1
1000000,00 1600000,00 2200000,00 0 1000 2000 3000 4000 5000
minutil minutil
Fig.3: Execution times
It can first be observed that mining PHUIs using PHM can be much faster
than mining HUIs. The reason for the excellent performance of PHM is that
it prunes a large part of the search space using its designed pruning strategies
basedonthemaximumandaverageperiodicitymeasures.Foralldatasets,itcan
be found that a huge amount of HUIs are non periodic, and thus pruning non

Retail Mushroom
25 FHM 10000 FHM
tnuoc
nrettaP
1
1 2
0
5 0 P P P H H H M M M 1 1 1 - - - 1 2 5 0 5 00 0 0 0 - - 5 5 -5 - -1 2 -5 5 00 0 00 tnuoc
nrettaP
1
1
0
1
0
00
0
0
P P H H M M 1 1 - - 1 5 0 0 0 0 0 -5 -5 -2 -5 50 00
5
1
0 10000 20000 30000 40000 50000 0 2000000 4000000 6000000 8000000 10000000
minutil minutil
Chaintstore Foodmart
450 1 000 000
FHM
400
PHM 1-5000-5-500 100 000
350
PHM 1-2000-5-200
tnuoc
nrettaP
1
1 2
2
3
0
5 0
5
0
0
0 0
0
0
tnuoc
nrettaP
1
1
0
1
0
0
0
1
0
00
0
0
0 F
P
P
H
H
H
M
M
M
1
1
0
-2
-5
0
0
0
0
0
0
-5
-5
-2
-5
00
00
50 PHM 10-1000-5-100
0 1
1000000,00 1600000,00 2200000,00 0 1000 2000 3000 4000 5000
minutil minutil
Fig.4: Number of patterns found
periodic patterns leads to a massive performance improvement. For example,
for the lowest minutil, maxPer and maxAvg values on these datasets, PHM is
respectively up to 214, 127, 100 and 230 times faster than FHM. In general, the
more the periodicity thresholds are restrictive, the more the gap between the
runtime of FHM and PHM increases.
A second observation is that the number of PHUIs can be much less than
the number of HUIs (see Fig. 4). For example, on retail, 20,714 HUIs are found
for minutil=2,000. But only 110 HUIs are PHUIs for PHM 1-1000-5-500, and
only 7 for PHM 1-250-5-150. Some of the patterns found are quite interesting
as they contains several items. For example, it is found that items with product
ids 32, 48 and 39 are periodically bought with an average periodicity of 16.32,
a minimum periodicity of 1, and a maximum periodicity of 170. Huge reduction
in the number of patterns are also observed on the other datasets. These overall
results show that the proposed PHM algorithm is useful as it can filter a huge
amount of non periodic HUIs encountered in real datasets, and can run faster.
Memory consumption was also compared, although detailed results are not
shown asafigure duetospace limitation.It wasobserved thatPHMcan useup
to 10 times less memory than FHM depending on how parameters are set. For
example, on chainstore and minutil=1,000,000, FHM and PHM 1-5000-5-500
respectively consumes 1,631 MB and 159 MB of memory.
5 Conclusion
Thispaperexploredtheproblemofminingperiodichigh-utilityitemsets(PHUIs).
An efficient algorithm named PHM (Periodic High-utility itemset Miner) was
proposed to efficiently discover PHUIs using novel minimum and average peri-
odicity measures. An extensive experimental study with real-life datasets has

shown that PHM can be more than two orders of magnitude faster than FHM,
and discover more than two orders of magnitude less patterns by filtering non
periodicHUIs.SourcecodeofPHM,FHManddatasetscanbedownloadedfrom
http://goo.gl/Y6eBdz. For future work, we will consider designing alternative
algorithms to mine PHUIs.
References
1. Agrawal, R., Srikant, R.: Fast algorithms for mining association rules in large
databases. In: Proc. Int. Conf. Very Large Databases, pp. 487–499, (1994)
2. Amphawan, K., Lenca, P., Surarerks, A.: Mining top-k periodic-frequent pattern
fromtransactionaldatabaseswithoutsupportthreshold.In:Proc.3rdIntern.Conf.
on Advances in Information Technology, pp. 18–29 (2009)
3. Amphawan, K., Surarerks, A., Lenca, P.: Mining periodic-frequent itemsets with
approximateperiodicityusingintervaltransaction-idslisttree.In:Proc.2010Third
Intern. Conf. on Knowledge Discovery and Data Mining, pp. 245-248 (2010)
4. Fournier-Viger, P., Wu, C.-W., Zida, S., Tseng, V. S.: FHM: Faster high-utility
itemsetminingusingestimatedutilityco-occurrencepruning.In:Proc.21stIntern.
Symp. on Methodologies for Intell. Syst., pp. 83–92 (2014)
5. Lan, G. C., Hong, T. P., Tseng, V. S.: An efficient projection-based indexing ap-
proach for mining high utility itemsets. Knowl. and Inform. Syst. 38(1), 85–107
(2014)
6. Kiran,R.U.,Reddy,P.K.:MiningRarePeriodic-FrequentPatternsUsingMultiple
Minimum Supports. In: Proc. 15th Intern. Conf. on Management of Data (2009)
7. Uday,U.R.,Kitsuregawa,M.,Reddy,P.K.:EfficientDiscoveryofPeriodic-Frequent
Patterns in Very Large Databases. Journal of Systems and Software, 112, 110–121
(2015)
8. Song, W., Liu, Y., Li, J.: BAHUI: Fast and memory efficient mining of high utility
itemsetsbasedonbitmap.Intern.JournalofDataWarehousingandMining.10(1),
1–15 (2014)
9. Liu,M.,Qu,J.:Mininghighutilityitemsetswithoutcandidategeneration.In:Proc.
22nd ACM Intern. Conf. Info. and Know. Management, pp. 55–64 (2012)
10. Liu,Y.,Liao,W.,Choudhary,A.:Atwo-phasealgorithmforfastdiscoveryofhigh
utility itemsets. In: Proc. 9th Pacific-Asia Conf. on Knowl. Discovery and Data
Mining, pp. 689–695 (2005)
11. Surana, A., Kiran, R. U., Reddy, P. K.: An efficient approach to mine periodic-
frequentpatternsintransactionaldatabases.In:Proc.2011QualityIssues,Measures
of Interestingness and Evaluation of Data Mining Models Workshop, pp. 254–266
(2012)
12. Tanbeer, S. K., Ahmed, C. F., Jeong, B. S., Lee, Y. K.: Discovering periodic-
frequentpatternsintransactionaldatabases.In:Proc.13thPacific-AsiaConference
on Knowledge Discovery and Data Mining, pp. 242–253 (2009)
13. Tseng, V. S., Shie, B.-E., Wu, C.-W., Yu., P. S.: Efficient algorithms for mining
high utility itemsets from transactional databases. IEEE Trans. Knowl. Data Eng.
25(8), 1772–1786 (2013)
