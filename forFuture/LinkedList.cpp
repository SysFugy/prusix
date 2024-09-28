#include <iostream>
template<class T> class LLNode{
    public:
        T data;
        LLNode<T>* next;
};

template<class T> class LinkedList{
    private:
        LLNode<T>* head = nullptr;
    public:
        void insert(T data){
            if(this->head == nullptr){
                this->head = new LLNode<T>();
                this->head->data = data;
            }
            else{
                LLNode<T>* elem = new LLNode<T>();

                elem->data = data;
                elem->next = this->head;
                this->head = elem;
            }
        }

        void print(){
            LLNode<T>* nodePtr = this->head;

            do{std::cout << nodePtr->data << std::endl;}while((nodePtr = nodePtr->next) != nullptr);
        }

};

int main(){
    LinkedList<int> l;

    for(int i = 0; i < 100; i++){
        l.insert(i);
    }

    l.print();
}
