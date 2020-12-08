#ifndef LISTA__H
#define LISTA__H

#include <iostream>

using namespace std;

/*Clase lista: lista doblemente enlazada. Contiene un iterador
 * con el que se puede recorrer la lista. */
 
template <typename T>
class lista{
  /* Clase nodo: es la unidad basica de la que esta
   * compuesta la lista*/
  class nodo{
    friend class lista;
    /* Punteros al nodo siguiente y el anterior*/
    nodo* sig;
    nodo* ant;
    T dato; //Dato

  public:
    nodo();
    nodo(const T&);
    ~nodo();
  };

  /* Punteros al primer y ultimo elemento de la lista */
  nodo* pri;
  nodo* ult;
  size_t tam;

public:
  /*Clase iterador: permite iterar por la lista de manera sencilla*/
  class iterador{
    friend class lista;

    nodo *actual; //Puntero al nodo al que apunta actualmente el iterador
    
    iterador(nodo*); //Constructor al que se le pasa el nodo al que se quiere apuntar.

  public:
    //Constructores
    iterador(); //Por defecto
    iterador(const lista<T>&); //Inicializa a principio de lista
    iterador(const iterador&); //Por copia

    ~iterador();

    T& dato(); //Devuelve dato de nodo actual (permite editar).
    const T& dato() const; //Devuelve dato de nodo actual
    iterador &avanzar(); //Avanza al nodo siguiente
    iterador &retroceder(); //Retrocede al nodo anterior
    bool extremo() const; //Chequea si llego a un extremo

    bool operator==(const iterador&) const;
    bool operator!=(const iterador&) const;
    const iterador& operator=(iterador const&);


  };

  //Definiciones de tipos.
  typedef T t_dato;
  typedef nodo t_nodo;
  typedef iterador t_iter;

  //Constructores
  lista(); //Defecto
  lista(const lista&); //Copia

  ~lista();

  lista<T> & operator=(const lista<T> &);
  bool vacia() const; //Chequea si esta vacia
  bool llena() const; //Chequea si esta llena
  size_t Tamano() const; //Devuelve el tamano
  void enqueue(const T&); //Agrega un dato al principio de la lista
  T dequeue(); //Extrae el dato del final de la lista
  
  iterador primero() const; //Devuelve un iterador al principio de la lista
  iterador ultimo() const; //Devuelve un iterador al final de la lista

  template<typename U> 
  friend std::ostream& operator<<(std::ostream&, const lista<U>&);
};


template <typename T>
lista<T>::nodo::nodo(){
  ant = 0;
  sig = 0;
}

template <typename T>
lista<T>::nodo::nodo(const T& dato_){
  ant = 0;
  sig = 0;
  dato = dato_;
}

template <typename T>
lista<T>::nodo::~nodo(){
}

template <typename T>
lista<T>::iterador::iterador(){
  actual = 0;
}

template <typename T>
lista<T>::iterador::iterador(nodo* actual_){
  actual = actual_;
}

template <typename T>
lista<T>::iterador::iterador(const lista<T>& l){
  actual = l.pri;
}

template <typename T>
lista<T>::iterador::iterador(const iterador& it){
  actual = it.actual;
}

template <typename T>
lista<T>::iterador::~iterador(){
}

template <typename T>
T& lista<T>::iterador::dato(){
  return actual->dato;
}

template <typename T>
const T& lista<T>::iterador::dato() const{
  return actual->dato;
}

template <typename T>
typename lista<T>::iterador& lista<T>::iterador::avanzar(){
  if(actual)
    actual = actual->sig;
  return *this;
}

template <typename T>
typename lista<T>::iterador& lista<T>::iterador::retroceder(){
  if(actual)
    actual = actual->ant;
  return *this;
}

template <typename T>
bool lista<T>::iterador::extremo() const{
  return !actual ? true : false;
}

template <typename T>
bool lista<T>::iterador::operator==(const typename lista<T>::iterador& it2) const{
  return actual == it2.actual;
}

template <typename T>
bool lista<T>::iterador::operator!=(const typename lista<T>::iterador& it2) const{
  return actual != it2.actual;
}

template <typename T>
const typename lista<T>::iterador& lista<T>::iterador::operator=(const typename lista<T>::iterador& orig){
  if(*this != orig){
    actual = orig.actual;
  }
  return *this;
}


template <typename T>
lista<T>::lista(){
  pri = 0;
  ult = 0;
  tam = 0;
}

template <typename T> 
lista<T>::lista(const lista& orig){
  nodo* p;
  nodo* ant;
  pri = 0;
  ult = 0;
  tam = orig.tam;

  for(p = orig.pri, ant = 0; p; p = p->sig){
    nodo* nuevo = new nodo(p->dato);
    nuevo->ant = ant;
    nuevo->sig = 0;

    if(ant)
      ant->sig = nuevo;


    if(!pri)
      pri = nuevo;

    ant = nuevo;
  }
  ult = ant;
}

template <typename T>
lista<T>::~lista(){
  for(nodo* p = pri; p; ){
    nodo * q = p->sig;
    delete p;
    p = q;
  }
}


template <typename T>
lista<T> & lista<T>::operator=(const lista<T> & orig) {
  if (&orig != this) {
    // libero memoria del this
    for(nodo* p = pri; p; ){
      nodo * q = p->sig;
      delete p;
      p = q;
    }
    // copio uno los nodos de orig this

    nodo* ptr;
    nodo* ant;
    pri = 0;
    ult = 0;
    tam = orig.tam;

    for(ptr = orig.pri, ant = 0; ptr; ptr = ptr->sig){
      nodo* nuevo = new nodo(ptr->dato);
      nuevo->ant = ant;
      nuevo->sig = 0;

      if(ant)
        ant->sig = nuevo;

      if(!pri)
        pri = nuevo;

      ant = nuevo;
    }
    ult = ant;
  }
  return *this;
}

template <typename T>
bool lista<T>::vacia() const{
  return pri ? false : true;
}

template <typename T>
bool lista<T>::llena() const{
  return pri ? true : false;
}

template <typename T>
size_t lista<T>::Tamano() const{
  return tam;
}


template <typename T>
void lista<T>::enqueue(const T& dato_){
  nodo * aux = new nodo(dato_);
  aux->sig = 0;
  aux->ant = ult;

  if(ult)
    ult->sig = aux;
  ult = aux;
  if(!pri)
    pri = aux;
  tam++;
}

template <typename T>
T lista<T>::dequeue(){
  if(tam < 1)
    throw std::logic_error("");
  T dato_ = ult->dato;

  nodo* p = ult;
  ult = p->ant;

  if(!ult)
    pri = 0;
  else
    ult->sig = 0;

  tam--;
  delete p;
  return dato_;
}



template <typename T>
std::ostream& operator<<(std::ostream& os, const lista<T>& l){
  if(!l.tam){
    os << "Lista vacia";
    return os;
  }

  typename lista<T>::iterador it(l);

  while(!it.extremo()){
    os << it.dato() << " ";
    it.avanzar();
  }
  return os;

}

template <typename T>
typename lista<T>::iterador lista<T>::primero() const{
  return typename lista<T>::iterador(pri);
}

template <typename T>
typename lista<T>::iterador lista<T>::ultimo() const{
  return typename lista<T>::iterador(ult);
}


#endif
