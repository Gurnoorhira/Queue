// CMSC 341 - Spring 2022 - Project 3
//Gurnoor Hira
//mytest.cpp
#include "squeue.h"
#include <random>
// the followings are sample priority functions to be used by SQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a post.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Post &post);// works with a MAXHEAP
int priorityFn2(const Post &post);// works with a MINHEAP

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:
    void checkMinHeap(Node* m_heap, SQueue cur, bool &flag);
    Node* getRoot(SQueue curr);
};

int main(){
    Tester tester;
    try{
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    SQueue queue2(priorityFn1,MAXHEAP);
    SQueue queue3(priorityFn2, MINHEAP);
    SQueue queue4(priorityFn2,MAXHEAP);
    //populate queue 3 and 4 for merge testing, keep same priority different heap type
    for (int i=0;i<5;i++){
        Post aPost(idGen.getRandNum(),
                    likesGen.getRandNum(),
                    conLevelGen.getRandNum(),
                    timeGen.getRandNum(),
                    interestGen.getRandNum());
        queue3.insertPost(aPost);
    }
    for (int i=0;i<5;i++){
        Post aPost(idGen.getRandNum(),
                    likesGen.getRandNum(),
                    conLevelGen.getRandNum(),
                    timeGen.getRandNum(),
                    interestGen.getRandNum());
        queue4.insertPost(aPost);
    }
    //test insertion
    cout << "Testing insertion normal case (300 insertions)" << endl;
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),
                    likesGen.getRandNum(),
                    conLevelGen.getRandNum(),
                    timeGen.getRandNum(),
                    interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    cout << "\nDump of the queue with priorityFn2 (MINHEAP):\n";
    queue1.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn1 (MINHEAP):\n";
    queue1.printPostsQueue();
    if(queue1.numPosts() == 300){
        cout <<"NORMAL INSERTION SUCCESSFUL FOR MINHEAP" << endl;
    }
    else{
        cout <<"NORMAL INSERTION UNSUCCESSFUL FOR MINHEAP" << endl;
    }
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),
                    likesGen.getRandNum(),
                    conLevelGen.getRandNum(),
                    timeGen.getRandNum(),
                    interestGen.getRandNum());
        queue2.insertPost(aPost);
    }
    cout << "\nDump of the queue with priorityFn1 (MAXHEAP):\n";
    queue2.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn1 (MAXHEAP):\n";
    queue2.printPostsQueue();
    if(queue2.numPosts() == 300){
        cout <<"NORMAL INSERTION SUCCESSFUL FOR MAXHEAP" << endl;
    }
    else{
        cout << "NORMAL INSERTION UNSUCCESSFUL FOR MAXHEAP" << endl;
    }
    cout << endl;
    //test merge
    cout << "MERGE MIN HEAP WITH MAX HEAP" << endl;
    queue3.mergeWithQueue(queue4);
    queue3.dump();
    queue3.printPostsQueue();
    if(queue3.numPosts() == 10){
        cout << "MERGE WORKED" << endl;
    }
    else{
        cout << "MERGE NOT WORK" << endl;
    }

    cout << endl;
    //test if heaptype remains
    cout <<"TEST IF PROPERTIES MAINTAINED" << endl;
    Node* temp = tester.getRoot(queue2);
    bool test = true;
    tester.checkMinHeap(temp,queue2,test);
    if(!test){
        cout << "PASSED, PROPERTIES MAINTAINED" << endl;
    }
    else{
        cout <<"NOT MAINTAINED" << endl;
    }

    cout << endl;
    //test copy constructor
    cout << "TEST COPY CONSTRUCTOR NORMAL CASE" << endl;
    SQueue queue10(queue3);
    cout <<"items of queue3 copied into queue 10" << endl;
    queue10.printPostsQueue();
    cout << endl;
//test assignment operator
    cout <<"TEST ASSIGNMENT OPERATOR" <<endl;
    SQueue queue11(priorityFn2,MINHEAP);
    queue11 = queue3;
    cout <<"assigned queue 3 to queue 11" << endl;
    queue11.printPostsQueue();
    cout << endl;

    return 0;


    }catch(std::out_of_range const &e){
        cout << endl;
    }
}

int priorityFn1(const Post &post) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-510]
    //the highest priority would be 500+10 = 510
    //the lowest priority would be 0+1 = 1
    //the larger value means the higher priority
    int priority = post.getNumLikes() + post.getInterestLevel();
    return priority;
}

int priorityFn2(const Post &post) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [2-55]
    //the highest priority would be 1+1 = 2
    //the lowest priority would be 5+50 = 55
    //the smaller value means the higher priority
    int priority = post.getPostTime() + post.getConnectLevel();
    return priority;
}
void Tester::checkMinHeap(Node* m_heap, SQueue cur, bool &flag){
    if(m_heap == nullptr){
        return;
    }
    if(m_heap->m_left && cur.m_priorFunc(m_heap->getPost()) > cur.m_priorFunc(m_heap->m_left->getPost())){
        flag = false;
    }
    if(m_heap->m_right && cur.m_priorFunc(m_heap->getPost()) > cur.m_priorFunc(m_heap->m_right->getPost())){
        flag = false;
    }
    checkMinHeap(m_heap->m_left,cur, flag);
    checkMinHeap(m_heap->m_right,cur, flag);
}
Node* Tester::getRoot(SQueue curr){
    if(curr.m_heap == nullptr){
        return nullptr;
    }
    return curr.m_heap;

}