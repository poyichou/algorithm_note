#include <stdio.h>
#include <stdlib.h>
/*
  Gale-Shapley Algorithm
  The Stable Matching Problem
  idea: men keep proposing women
  status of each person: free, engaged, married
  male-optimal and female-pessimal
  stable matching is not unique

  For n men, n women
  time complexity: O(n^2)
*/
/*
=============== Preference List =============
        1st     2nd     3rd     4th     5th
# Men's Preference Profile
Vitor:  Bertha  Amy     Diane   Erika   Clare
Wyatt:  Diane   Bertha  Amy     Clare   Erika
Xavier: Bertha  Erika   Clare   Diane   Amy
Yancey: Amy     Diane   Clare   Bertha  Erika
Zeus:   Bertha  Diane   Amy     Erika   Clare
=============================================
# Women's Preference Profile
Amy:    Zeus    Vitor   Wyatt   Yancey  Xavier
Bertha: Xavier  Wyatt   Yancey  Vitor   Zeus
Clare:  Wyatt   Xavier  Yancey  Zeus    Vitor
Diane:  Vitor   Zeus    Yancey  Xavier  Wyatt
Erika:  Yancey  Wyatt   Zeus    Xavier  Vitor
==============================================
*/

typedef struct male Man;
typedef struct female Woman;
struct male{
        char name;
        int propose_idx;
        Woman *fiancee;
        char p_list[5];

        Man *prev;
        Man *next;
};
struct female{
        char name;
        Man *fiance;
        char p_list[5];
};
// NOTE
static Man *free_men = NULL, *engaged_men = NULL;
static Woman **women = NULL;

void print_real_name(char name) {
        switch (name) {
                //man
                case 'V':
                        printf("Vitor");
                        break;
                case 'X':
                        printf("Xavier");
                        break;
                case 'Y':
                        printf("Yancey");
                        break;
                case 'Z':
                        printf("Zeus");
                        break;
                case 'W':
                        printf("Wyatt");
                        break;
                //woman
                case 'A':
                        printf("Amy");
                        break;
                case 'B':
                        printf("Bertha");
                        break;
                case 'C':
                        printf("Clare");
                        break;
                case 'D':
                        printf("Diane");
                        break;
                case 'E':
                        printf("Erika");
                        break;
                default:
                        printf("error\n");
                        exit(1);

        }
}

void init_male(char name, char p0, char p1, char p2, char p3, char p4){
        Man *man = malloc(sizeof(Man));
        man->name = name;
        man->propose_idx = 0;
        man->fiancee = NULL;
        man->p_list[0] = p0;
        man->p_list[1] = p1;
        man->p_list[2] = p2;
        man->p_list[3] = p3;
        man->p_list[4] = p4;
        //set man as free
        man->prev = NULL;
        man->next = free_men;
        if (free_men) {
                free_men->prev = man;
        }
        free_men = man;
}
void create_male() {
        //Vitor:  Bertha  Amy     Diane   Erika   Clare
        init_male('V', 'B', 'A', 'D', 'E', 'C');
        //Wyatt:  Diane   Bertha  Amy     Clare   Erika
        init_male('W', 'D', 'B', 'A', 'C', 'E');
        //Xavier: Bertha  Erika   Clare   Diane   Amy
        init_male('X', 'B', 'E', 'C', 'D', 'A');
        //Yancey: Amy     Diane   Clare   Bertha  Erika
        init_male('Y', 'A', 'D', 'C', 'B', 'E');
        //Zeus:   Bertha  Diane   Amy     Erika   Clare
        init_male('Z', 'B', 'D', 'A', 'E', 'C');
}
void init_female(char name, char p0, char p1, char p2, char p3, char p4){
        static int idx = 0;
        Woman* woman = malloc(sizeof(Woman));
        woman->fiance = NULL;
        woman->name = name;
        // NOTE
        // 1~5
        woman->p_list[p0 - 'V'] = 1;
        woman->p_list[p1 - 'V'] = 2;
        woman->p_list[p2 - 'V'] = 3;
        woman->p_list[p3 - 'V'] = 4;
        woman->p_list[p4 - 'V'] = 5;
        women[idx++] = woman;
}
void create_female() {
        women = malloc(sizeof(Woman*) * 5);
        //Amy:    Zeus    Vitor   Wyatt   Yancey  Xavier
        init_female('A', 'Z', 'V', 'W', 'Y', 'X');
        //Bertha: Xavier  Wyatt   Yancey  Vitor   Zeus
        init_female('B', 'X', 'W', 'Y', 'V', 'Z');
        //Clare:  Wyatt   Xavier  Yancey  Zeus    Vitor
        init_female('C', 'W', 'X', 'Y', 'Z', 'V');
        //Diane:  Vitor   Zeus    Yancey  Xavier  Wyatt
        init_female('D', 'V', 'Z', 'Y', 'X', 'W');
        //Erika:  Yancey  Wyatt   Zeus    Xavier  Vitor
        init_female('E', 'Y', 'W', 'Z', 'X', 'V');
}
// for women
int prefer(Woman *woman, char m1, char m2) {
        if (woman->p_list[m1 - 'V'] > woman->p_list[m2 - 'V']) {
                return 1;
        } else if (woman->p_list[m1 - 'V'] < woman->p_list[m2 - 'V']) {
                return -1;
        } else {
                return 0;
        }
}
void print_pairs() {
        Man* list = engaged_men;
        while (list) {
                print_real_name(list->name);
                printf("\tmarried\t");
                print_real_name(list->fiancee->name);
                printf("\n");
                list = list->next;
        }
}
//extract man from *from_list and add to *to_list
void move_man(Man *man, Man **from_list, Man **to_list) {
        //extract man from *from_list
        if (man->prev) {
                man->prev->next = man->next;
        } else {
                //*from_list == man
                *from_list = (*from_list)->next;
        }
        if (man->next) {
                man->next->prev = man->prev;
        }
        //add to *to_list
        man->prev = NULL;
        man->next = *to_list;
        if (*to_list) {
                (*to_list)->prev = man;
        }
        *to_list = man;
}
void accept_proposal(Man *man, Woman *woman) {
        woman->fiance = man;
        man->fiancee = woman;
        //extract man from free_men
        move_man(man, &free_men, &engaged_men);
}
void reject_fiance(Woman *woman) {
        //reject fiance
        Man *poor_man = woman->fiance;
        if (poor_man == NULL) {
                printf("error\n");
                exit(1);
        }
        woman->fiance = NULL;
        poor_man->fiancee = NULL;
        //extract poor man from engaged_men
        move_man(poor_man, &engaged_men, &free_men);
}
void make_pairs() {
        Man *man;
        Woman *woman;
        // NOTE
        while ((man = free_men)) {
                woman = women[man->p_list[man->propose_idx] - 'A'];
                if (woman == NULL) {
                        printf("error");
                        exit(1);
                } else if (woman->fiance == 0) {
                        //free, just accept
                        accept_proposal(man, woman);
                } else if (prefer(woman, man->name, woman->fiance->name) == 1) {
                        //engaged, woman can change fiance base on her preference
                        reject_fiance(woman);
                        accept_proposal(man, woman);
                }
                man->propose_idx++;
        }
}
void free_all() {
        Man *free_m_list;
        while ((free_m_list = engaged_men)) {
                engaged_men = engaged_men->next;
                free(free_m_list);
        }
        while ((free_m_list = free_men)) {
                free_men = free_men->next;
                free(free_m_list);
        }
        int i;
        for (i = 0; i < 5; i++) {
                free(women[i]);
        }
        free(women);
}
int main()
{
        create_male();
        create_female();
        make_pairs();
        print_pairs();
        free_all();
        return 0;
}
