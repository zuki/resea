/** @file list.h */
#ifndef __LIST_H__
#define __LIST_H__

#include <print_macros.h>
#include <types.h>


/** @ingroup common
 * @def LIST_CONTAINER
 * @brief listからfieldをcontainer型で取り出す
 */
#define LIST_CONTAINER(head, container, field)                                 \
    ((container *) ((vaddr_t)(head) -offsetof(container, field)))

/** @ingroup common
 * @def LIST_FOR_EACH
 * @brief listの全要素を走査してfor eachを実行する.
 * <pr>
 * Usage:
 *
 *  struct element {
 *     struct list_head next;
 *     int foo;
 * };
 *
 * LIST_FOR_EACH(elem, &elems, struct element, next) {
 *     printf("foo: %d", elem->foo);
 * }
 * </pr>
 */
#define LIST_FOR_EACH(elem, list, container, field)                            \
    for (container *elem = LIST_CONTAINER((list)->next, container, field),     \
                   *__next = NULL;                                             \
         (&elem->field != (list)                                               \
          && (__next = LIST_CONTAINER(elem->field.next, container, field)));   \
         elem = __next)

/** @ingroup common
 * @def struct list_head
 * @brief 双方向リストを表す構造体.
 */
struct list_head {
    struct list_head *prev;     /**< 前要素へのポインタ */
    struct list_head *next;     /**< 次要素へのポインタ */
};

/** @ingroup common
 * @typedef list_t
 * @brief struct list_headの型定義（リストに注目）.
 */
typedef struct list_head list_t;
/** @ingroup common
 * @typedef list_elem_t
 * @brief struct list_headの型定義（リスト要素に注目）.
 */
typedef struct list_head list_elem_t;

/** @ingroup common
 * @brief リストが空であるか否かをチェックする.
 * @param list チェックするリストへのポインタ
 * @return 空なら true; そうでなければ false
 */
static inline bool list_is_empty(list_t *list) {
    return list->next == list;
}

/** @ingroup common
 * @brief リストの長さを取得する.
 * @param list リストへのポインタ
 * @return リストの長さ
 */
static inline size_t list_len(list_t *list) {
    size_t len = 0;
    struct list_head *node = list->next;
    while (node != list) {
        len++;
        node = node->next;
    }

    return len;
}

/** @ingroup common
 * @brief リストに指定の要素が含まれているかチェックする.
 * @param list リストへのポインタ
 * @param elem 要素へのポインタ
 * @return 含まれていれば true; 含まれていなければ false
 */
static inline bool list_contains(list_t *list, list_elem_t *elem) {
    list_elem_t *node = list->next;
    while (node != list) {
        if (node == elem) {
            return true;
        }
        node = node->next;
    }

    return false;
}

/** @ingroup common
 * @brief `prev` と `next` の間に新要素を挿入する.
 * @param prev 新要素の前要素となる要素
 * @param next 新要素の次要素となる要素
 * @param new 挿入する新要素
 */
static inline void list_insert(list_elem_t *prev, list_elem_t *next,
                               list_elem_t *new) {
    new->prev = prev;
    new->next = next;
    next->prev = new;
    prev->next = new;
}

/** @ingroup common
 * @brief リストを初期化する.
 * @param list 初期化するリストへのポインタ
 */
static inline void list_init(list_t *list) {
    list->prev = list;
    list->next = list;
}

/** @ingroup common
 * @brief リスト要素を無効にする.
 * @param elem 無効にする要素へのポインタ
 */
static inline void list_nullify(list_elem_t *elem) {
    elem->prev = NULL;
    elem->next = NULL;
}

/** @ingroup common
 * @brief リストから要素を削除する.
 * @param elem 削除する要素へのポインタ
 */
static inline void list_remove(list_elem_t *elem) {
    if (!elem->next) {
        // 要素はリストにない
        return;
    }
    // 1. 付け替え
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;

    // 2. 要素をリストから外したので無効にする.
    list_nullify(elem);
}

/** @ingroup common
 * @brief リストの末尾に要素を追加する.
 * @param list リストへのポインタ
 * @param new_tail 追加する要素へのポインタ
 */
static inline void list_push_back(list_t *list, list_elem_t *new_tail) {
    DEBUG_ASSERT(!list_contains(list, new_tail));
    list_insert(list->prev, list, new_tail);
}

/** @ingroup common
 * @brief リストの先頭要素を取り出す.
 * @param list リストへのポインタ
 * @return 取り出した要素へのポインタ. リストが空の場合は NULL.
 */
static inline list_t *list_pop_front(list_t *list) {
    // 1. 先頭要素を取り出す
    struct list_head *head = list->next;
    if (head == list) {
        return NULL;
    }

    // 2. 戦闘要素をリストから外す
    // list <-> head <-> next => list <-> next
    struct list_head *next = head->next;
    list->next = next;
    next->prev = list;

    // 3. 先頭要素をリストから外したので無効にする.
    list_nullify(head);
    return head;
}

/** @ingroup common
 * @def LIST_POP_FRONT
 * @brief リストの先頭要素の指定したフィールドを取り出す.
 * <pre>
 * list リストへのポインタ
 * container 取り出したいフィールドの型
 * field 取り出したいフィールド
 * return 取り出したフィールドへのポインタ. リストが空の場合は NULL.
 * </pre>
 */
#define LIST_POP_FRONT(list, container, field)                                 \
    ({                                                                         \
        list_elem_t *__elem = list_pop_front(list);                            \
        (__elem) ? LIST_CONTAINER(__elem, container, field) : NULL;            \
    })

#endif
