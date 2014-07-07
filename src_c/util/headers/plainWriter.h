#ifndef __PLAIN_WRITER__
#define __PLAIN_WRITER__

#include <cstdlib>

#define PW_DEFAULT_SIZE 4 //1024

template<class T>
class PlainWriter{
private:
    T* _buffers[2];

    int _cur_buffer,_old_buffer;
    int _cursor;
    int _last_element;
    int _size;

    bool _firstTime;

    void increaseSize();
    void increaseSize(int n);
    void checkSize(int length);
public:
    PlainWriter():_cursor(0), _size(0),_cur_buffer(0),_old_buffer(1),_firstTime(true),_last_element(0){_buffers[0]=NULL;_buffers[1]=NULL;};
    ~PlainWriter(){delete[] _buffers[0]; delete[] _buffers[1];};

    void write(T data);
    void writeLine(T* data, const int length);
    void writeArray(T** data, const int w, const int h);

    T read();
    T* readLine(const int length);
    T** readArray(const int w, const int h);

    PlainWriter& moveBegin();
    PlainWriter& moveEnd();
};

template<class T>
void PlainWriter<T>::increaseSize(){
    if(_size==0) _size = PW_DEFAULT_SIZE;
    increaseSize(_size*2);
}

template<class T>
void PlainWriter<T>::increaseSize(const int n){
    _cur_buffer = (_cur_buffer+1)%2;
    _old_buffer = (_old_buffer+1)%2;

    if(_buffers[_cur_buffer]!=NULL) delete _buffers[_cur_buffer];
    _buffers[_cur_buffer] = new T[n];

    if(!_firstTime){
        for(int i=0;i<_size;i++){
            _buffers[_cur_buffer][i] = _buffers[_old_buffer][i];
        }
    }

    _size = n;
    _firstTime = false;
}

template<class T>
void PlainWriter<T>::checkSize(int length){
    while(_cursor+length >= _size) increaseSize();
}

template<class T>
void PlainWriter<T>::write(T data){
    checkSize(1);
    _buffers[_cur_buffer][_cursor++] = data;    
}

template<class T>
void PlainWriter<T>::writeLine(T* data, int length){
    checkSize(length);
    for(int i=0;i<length;i++){
        _buffers[_cur_buffer][_cursor++]=data[i];
    }
    _last_element = _cursor;
}

template<class T>
void PlainWriter<T>::writeArray(T** data, int w, int h){
    checkSize(h*w);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            _buffers[_cur_buffer][_cursor++] = data[i][j];
        }
    }
    _last_element = _cursor;
}

template<class T>
T PlainWriter<T>::read(){
    return _buffers[_cur_buffer][_cursor++];
}

template<class T>
T* PlainWriter<T>::readLine(const int length){
    T* ret_data = new T[length];
    for(int i=0;i<length;i++){
        ret_data[i] = _buffers[_cur_buffer][_cursor++];
    }

    return ret_data;
}

template<class T>
T** PlainWriter<T>::readArray(const int w, const int h){
    T** ret_data = new T*[h];

    for(int i=0;i<h;i++){
        ret_data[i] = new T[w];
    }

    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            ret_data[i][j] = _buffers[_cur_buffer][_cursor++];
        }
    }

    return ret_data;
}

template<class T>
PlainWriter<T>& PlainWriter<T>::moveBegin(){
    _cursor = 0;
    return *this;
}
    
template<class T>
PlainWriter<T>& PlainWriter<T>::moveEnd(){
    _cursor = _last_element = _cursor;
    return *this;
}

#endif
