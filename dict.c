#include <assert.h>
#include "dict.h"
#include <stdlib.h>
#include "string.h"

struct _node_t {
    dict_t left;
    dict_t right;
    key_t key;
    value_t value;
};

dict_t dict_empty() {
    dict_t dict = NULL;
    assert(dict_length(dict) == 0);
    return dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(word != NULL && def != NULL);
    if (dict == NULL){
        dict = calloc(1, sizeof(struct _node_t));
        dict-> value = def;
        dict-> key = word;
        dict-> left = NULL;
        dict-> right = NULL;
    }
    else{
        if (key_less(dict->key, word)){
            dict-> right = dict_add(dict->right, word, def);
        }else if (key_less(word, dict->key)){
            dict-> left = dict_add(dict->left, word, def);
        }else if(key_eq(dict->key, word)){
            dict-> value = def;
        }
    }
    assert(value_eq(def, dict_search(dict, word)));
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    key_t def = NULL;
    assert(word != NULL);
    if(dict_exists(dict, word)){
        if (key_less(dict->key, word)){
            def = dict_search(dict->right, word);
        }else if (key_less(word, dict->key)){
            def = dict_search(dict->left, word);
        }else if(key_eq(dict->key, word)){
            def = dict->value;
        }
    }
    assert((def==NULL && !dict_exists(dict, word)) || def != NULL);
    return def;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(word != NULL);
    bool b;
    if(dict == NULL){
        b = false;
    }else{
        if (key_less(dict->key, word)){
            b = dict_exists(dict->right, word);
        }else if (key_less(word, dict->key)){
            b = dict_exists(dict->left, word);
        }else if(key_eq(dict->key, word)){
            b = true;
        }
    }
    return b;
}

unsigned int dict_length(dict_t dict) {
    unsigned int n;
    if (dict == NULL){
        n = 0;
    }else {
        if (dict->left == NULL){
            if (dict->right == NULL){
                n = 1;
            }else {
                n = 1 + dict_length(dict->right);
            }
        }else {
            if (dict->right == NULL){
                n = 1 + dict_length(dict->left);
            }else {
                n = (dict_length(dict->left) + dict_length(dict->right)) + 1;
            }
        }
    }
    return n;
}

dict_t min_right(dict_t dict) {
    while (dict->left != NULL){
        dict = dict->left;
    }
    return dict;
} 

dict_t min_right_root(dict_t dict) {
    if(dict->right != NULL){
        if(dict->right->left != NULL){
            dict = dict->right;
            while(dict->left->left != NULL){
                dict = dict->left;
            }
        }
    }
    return dict;
}

dict_t dict_remove(dict_t dict, key_t word) {
    assert(word != NULL);
    dict_t var;
    dict_t root;
    var = dict;
        if(key_less(dict->key, word)){
            dict-> right = dict_remove(dict->right, word);
        }else if (key_less(word,dict->key)){
            dict-> left = dict_remove(dict->left, word);
        }else if (key_eq(dict->key, word)){
            if (dict->left == NULL){
                var = dict->right;
                dict->key = key_destroy(dict->key);
                dict->value = value_destroy(dict->value);
                free(dict);
                dict = NULL;
                return var;
            }else if (dict->right == NULL){
                var = dict->left;
                dict->key = key_destroy(dict->key);
                dict->value = value_destroy(dict->value);
                free(dict);
                dict = NULL;
                return var;
            }else if (dict->left != NULL && dict->right != NULL){
                if(dict->right->left == NULL){
                    var = min_right(dict->right);
                    root = min_right_root(dict);
                    dict->key = key_destroy(dict->key);
                    dict->value = value_destroy(dict->value);
                    dict->key = var->key;
                    dict->value = var->value;
                    root->right = var->right;
                    free(var);
                    var = NULL;
                }else{
                    var = min_right(dict->right);
                    root = min_right_root(dict);
                    if (root->left != NULL){
                        dict->key = key_destroy(dict->key);
                        dict->value = value_destroy(dict->value);
                        dict->key = var->key;
                        dict->value = var->value;
                        root->left = var->right;
                        free(var);
                        var = NULL;
                    }else{
                        dict->key = key_destroy(dict->key);
                        dict->value = value_destroy(dict->value);             
                        dict->key = var->key;
                        dict->value = var->value;
                        root->right = var->right;
                        free(var);
                        var = NULL;
                    }
                }
            }
        }
        assert(!dict_exists(dict, word));
    return dict;    
}

dict_t dict_remove_all(dict_t dict) {
    dict = dict_empty();
    assert(dict_length(dict) == 0);
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    if (dict != NULL){
    if (dict->left != NULL){
        dict_dump(dict->left,file);
    }
    key_dump(dict->key,file);
    fprintf(file,"=");
    value_dump(dict->value,file);
    fprintf(file," \n ");
    if (dict->right != NULL){
        dict_dump(dict->right,file);
    }
    }
}

dict_t dict_destroy(dict_t dict) {
    free(dict);
    dict = NULL;
    return dict;
}