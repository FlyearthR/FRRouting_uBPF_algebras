#include "hset.h"
#include "../ripng/sort.h"
#include "../../include/public_bpf.h"
#include <string.h>


struct set {
    int size;
    int str_length;
    int nb_null;
    int nb_elem;
    char* table[];
};

struct pair {
    unsigned int hash;
    char* string;
};

struct set* new_set(int s, int sl)
{
    struct set* r = (struct set*) ctx_calloc(sizeof(struct set) + s*sizeof(char*), sizeof(char));
    r->size = s;
    r->str_length = sl;
    //r->table = (char**) ctx_calloc(s, sizeof(char*));
    return r;
}

unsigned int hash(struct set* this, char* str) /* inspired from djb2 from http://www.cse.yorku.ca/~oz/hash.html */
{
    unsigned long hash = 5381;
    int max = (int) str+this->str_length;

    while ((int)str < max)
        hash = ((hash << 5) + hash) + *str++;

    return (unsigned int) (hash%this->size);
}

int contains(struct set* this, char* str) {
    unsigned int h = hash(this, str);
    unsigned int init = h;
    int i = 0;
    do {
        if(this->table[h] == NULL)
            i++;
        else if(strncmp(this->table[h], str, this->str_length))
            return 1;
        h++;
        h %= this->size;
    } while(i <= this->nb_null && h != init);
    return 0;
}

int insert(struct set* this, char* str) {
    if (this->size >= this->nb_elem)
        return 1;
    char* item = (char*) malloc(strlen(str)+1);
    strncpy(item, str, this->size);

    unsigned int h = hash(this, item);
    while(this->table[h] != NULL) {
        h++;
        h %= this->size;
    }

    this->table[h] = item;
    this->nb_null--;
    return 0;
}

int cmp(const void* p11, const void* p22)
{
    struct pair* p1 = (struct pair*) p11;
    struct pair* p2 = (struct pair*) p22;
    return p2->hash - p1->hash;
}
void complex_delete(struct set* this, char* str)
{
    unsigned int init = hash(this, str);
    unsigned int h = init;
    int i = 0;
    int index = 0;
    int last_null = -1;
    int lniao = 0; //Last Null Implies An Offset
    //TODO: list all the null value instead of just the last one to detect all the null that count in i
    int overcome = 0;
    struct pair table[this->size];

    while(i <= this->nb_null && h != init) {
        if (this->table[h] == NULL) {
            last_null = h;
            lniao = 0;
        }
        else {
            unsigned int new_h = hash(this, this->table[h]);
            if (new_h != h) {
                if (!lniao && (overcome ? (int) new_h <= last_null && new_h > h : (int) new_h <= last_null)) {
                    lniao = 1;
                    i++;
                }
                table[index].hash = new_h;
                table[index++].string = this->table[h];
                this->table[h] = NULL;
            }
        }
        h++;
        if (!overcome && h/this->size > 0) {
            h %= this->size;
            overcome = 1;
        }
    }

    MergeSort(table, sizeof(struct pair), index, &cmp);

    overcome = 0;
    int ii = table[0].hash;
    for (int j = 0 ; j < index ; j++) {
        while (this->table[ii] != NULL || table[j].hash < hash(this, this->table[ii]))
            ii++;
        this->table[ii++] = table[j].string;
    }
    this->nb_null -= i;
}
/*struct DataItem* delete(struct DataItem* item) {
    int key = item->key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {

        if(hashArray[hashIndex]->key == key) {
            struct DataItem* temp = hashArray[hashIndex];

            //assign a dummy item at deleted position
            hashArray[hashIndex] = dummyItem;
            return temp;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}*/