#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 256
#define ADD_TO_FRONT_LENGTH 6
#define INPUT_FILE_NAME "input.txt"

typedef struct Node {
    char* word;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct NodeDeque {
    char* word;
    struct NodeDeque* next;
} NodeDeque;

typedef struct Deque {
    NodeDeque* front;
    NodeDeque* rear;
} Deque;

Node* createNode(const char* word) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("ѕомилка при вид≥ленн≥ пам'€т≥ дл€ вузла списку.\n");
        exit(EXIT_FAILURE);
    }
    newNode->word = _strdup(word);
    newNode->next = newNode->prev = NULL;
    return newNode;
}

Deque* createDeque() {
    Deque* newDeque = (Deque*)malloc(sizeof(Deque));
    if (newDeque == NULL) {
        printf("ѕомилка при вид≥ленн≥ пам'€т≥ дл€ деку.\n");
        exit(EXIT_FAILURE);
    }
    newDeque->front = newDeque->rear = NULL;
    return newDeque;
}

NodeDeque* createNodeDeque(const char* word) {
    NodeDeque* newNode = (NodeDeque*)malloc(sizeof(NodeDeque));
    if (newNode == NULL) {
        printf("ѕомилка при вид≥ленн≥ пам'€т≥ дл€ вузла деку.\n");
        exit(EXIT_FAILURE);
    }
    newNode->word = _strdup(word);
    newNode->next = NULL;
    return newNode;
}

bool isGreaterUkr(const char* s1, const char* s2) {
    const char ukrAlphabet[] = "јаЅб¬в√г•іƒд≈е™Ї∆ж«з»и≤≥ѓњ…й кЋлћмЌнќоѕп–р—с“т”у‘ф’х÷ц„чЎшўщ№ьёюя€";
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    while (*p1 != '\0' && *p2 != '\0') {
        if (*p1 != *p2) {
            const char* q1 = strchr(ukrAlphabet, toupper(*p1));
            const char* q2 = strchr(ukrAlphabet, toupper(*p2));
            if (q1 != NULL && q2 != NULL) {
                return (q1 - ukrAlphabet <= q2 - ukrAlphabet);
            }
            else {
                return (*p1 > *p2);
            }
        }
        p1++;
        p2++;
    }
    return (*p1 != '\0');
}

void insertInOrder(Node** head, const char* word) {
    Node* newNode = createNode(word);
    Node** current = head;

    while (*current && !isGreaterUkr(word, (*current)->word)) {
        current = &(*current)->next;
    }

    newNode->next = *current;
    if (*current) {
        (*current)->prev = newNode;
    }
    *current = newNode;
}

void readFromFileAndInsertInOrder(Node** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("ѕомилка при в≥дкритт≥ файлу.\n");
        exit(EXIT_FAILURE);
    }

    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%255s", word) == 1) {
        insertInOrder(head, word);
    }

    fclose(file);
}

void addToDeque(Deque* deque, const char* word) {
    NodeDeque* newNode = createNodeDeque(word);
    if (deque->front == NULL) {
        deque->front = deque->rear = newNode;
    }
    else {
        if (strlen(word) < ADD_TO_FRONT_LENGTH) {
            newNode->next = deque->front;
            deque->front = newNode;
        }
        else {
            deque->rear = deque->rear->next = newNode;
        }
    }
}

void freeNodeAndWord(Node* node) {
    free(node->word);
    free(node);
}

void moveWordsToDeque(Node** head, Deque* deque) {
    Node* current = *head;
    while (current != NULL) {
        addToDeque(deque, current->word);
        Node* nextNode = current->next;
        freeNodeAndWord(current);
        current = nextNode;
    }
    *head = NULL;
}

void printNodes(Node* node) {
    for (; node; node = node->next) printf("%s ", node->word);
    printf("\n");
}

void printNodesDeque(NodeDeque* node) {
    for (; node; node = node->next) printf("%s ", node->word);
    printf("\n");
}

void freeNodeDeque(NodeDeque* node) {
    free(node->word);
    free(node);
}

void freeDeque(Deque* deque) {
    NodeDeque* current = deque->front;
    while (current != NULL) {
        NodeDeque* nextNode = current->next;
        freeNodeDeque(current);
        current = nextNode;
    }
    free(deque);
}

int main() {
    system("chcp 1251 & cls");
    Node* sortedList = NULL;
    readFromFileAndInsertInOrder(&sortedList, INPUT_FILE_NAME);
    printf("¬пор€дкований список:\n");
    printNodes(sortedList);

    Deque* wordDeque = createDeque();
    moveWordsToDeque(&sortedList, wordDeque);
    printf("\nƒек:\n");
    printNodesDeque(wordDeque->front);

    freeDeque(wordDeque);

    return 0;
}