// CMSC 341 - Spring 2022 - Project 3
//Gurnoor Hira
//squeue.cpp
#include "squeue.h"

SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType)
{
  m_heap = nullptr;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_size = 0;
}

SQueue::~SQueue(){
  if(m_size == 0){
    return;
  }
  m_size = 0;
  m_priorFunc = nullptr;
  deleteHelper(m_heap);
}


SQueue::SQueue(const SQueue& rhs){ // copy constructor copy everything over to rhs object
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heap = copyHelper(rhs.m_heap);
}

SQueue& SQueue::operator=(const SQueue& rhs){ //assign properties to rhs object
  if(this != &rhs){
    if(rhs.m_heap == nullptr){
      cout << "heap empty in equal operator" << endl;
    }
    else{
      clear();
      m_size = rhs.m_size;
      m_priorFunc = rhs.m_priorFunc;
      m_heap = copyHelper(rhs.m_heap);
    }
  }
  return *this;
}

void SQueue::insertPost(const Post& post){
  SQueue tempQueue(m_priorFunc,m_heapType);
  tempQueue.m_heap = new Node(post);
  tempQueue.m_size = 1;
  mergeWithQueue(tempQueue);
}

Post SQueue::getNextPost(){
  if(m_heap == nullptr){
    throw std::out_of_range("get next, heap empty");
  }
  else{
    Node *temp = m_heap;
    Node* temp2 = m_heap->m_right;
    m_heap = m_heap->m_left;
    temp->m_left = nullptr;
    temp->m_right = nullptr;
    m_heap = mergeHelper(m_heap,temp2);
    m_size--;
    Post post(temp->m_post);
    delete temp;
    temp = nullptr;
    temp2 = nullptr;
    return post;
  }
}

void SQueue::mergeWithQueue(SQueue& rhs) {
  if(rhs.m_size == 0){
    return;
  }
  else if(rhs.m_priorFunc != this->m_priorFunc){
    throw std::out_of_range("can't merge queues w diff priorities");
  }
  else{
    Node *temp = rhs.m_heap;
    m_size += rhs.m_size;
    rhs.m_size = 0;
    m_heap = mergeHelper(m_heap,temp);
  }
}

void SQueue::clear() {
  deleteHelper(m_heap);
}

int SQueue::numPosts() const
{
  return m_size;  
}

void SQueue::printPostsQueue() const {
  if(m_heap == nullptr){
    cout << "Can not print an empty heap"<< endl;
  }
  else{
    printHelper(m_heap);
  }
}

prifn_t SQueue::getPriorityFn() const {
  return m_priorFunc;
}

void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  m_priorFunc = priFn;
  m_heapType = heapType;
  Node* h = m_heap;
  m_heap = nullptr; 
  setHelper(h);
}

void SQueue::dump() const
{
  if (m_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }
}

void SQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_post) << ":" << pos->m_post.getPostID();
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Post& post){
  sout << "Post ID: " << post.getPostID() 
        << ", number of likes: " << post.getNumLikes()
        << ", interest level: " << post.getInterestLevel() 
        << ", connection level: " << post.getConnectLevel()
        << ", post time: " << post.getPostTime();
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node){
  sout << node.getPost();
  return sout;
}

void SQueue::deleteHelper(Node* node){
  if(node == nullptr){
    return;
  }
  deleteHelper(node->m_right);
  deleteHelper(node->m_left);
  delete node;
}

void SQueue::printHelper(Node* node) const{
  //recursive function to help print
  if(node == nullptr){
    return;
  }
  cout << node->getPost() << endl;
  printHelper(node->m_left);
  printHelper(node->m_right);

}
Node* SQueue::mergeHelper(Node *a, Node *b){
  //recursive function to help merge
  if(a == nullptr){
    return b;
  }
  if(b == nullptr){
    return a;
  }
  if(m_heapType == MINHEAP){
    if(m_priorFunc(a->getPost()) > m_priorFunc(b->getPost())){
      Node* y = a;
      a = b;
      b = y;
      y = nullptr;
    }
  }
  else{
    if(m_priorFunc(a->getPost()) < m_priorFunc(b->getPost())){
      Node* z = a;
      a = b;
      b = z;
      z = nullptr;
    }
  }
  Node* t = a->m_left;
  a->m_left = a->m_right;
  a->m_right = t;
  t = nullptr;
  a->m_left = mergeHelper(a->m_left,b);
  b = nullptr;
  return a;
}

Node* SQueue::copyHelper(Node* node){
  //recursive copy constructor
  if(node == nullptr){
    return nullptr;
  }
  Node *toInsert = new Node(node->getPost());
  toInsert->m_left = copyHelper(node->m_left);
  toInsert->m_right = copyHelper(node->m_right);
  return toInsert;
}

void SQueue::setHelper(Node* node){
  if(node == nullptr){
    return;
  }
  else{
    Node* left = node->m_left;
    Node* right = node->m_right;
    node->m_right = nullptr;
    node->m_left = nullptr;
    m_heap = mergeHelper(m_heap,node);
    setHelper(left);
    setHelper(right);
    left = nullptr;
    right = nullptr;
    node = nullptr;

  }
}

