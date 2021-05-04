#include "PoolAllocator.h"

/*
template <typename T>
PoolAllocator<T>::PoolAllocator(int totalObjects):totalobjects_(totalObjects) {
    MPOD_ASSERT(objects_.empty());
    objects_.insert(objects_.begin(), totalobjects_, nullptr);

    for(int i=0; i<totalobjects_; ++i) {
        objects_[i]=new T();
    }
}

template <typename T>
PoolAllocator<T>::~PoolAllocator() {
    Release();
}

template <typename T>
void PoolAllocator<T>::Render() {
    //SetupMaterial(1,1,1);
    for(int i=0; i<(int)objects_.size(); ++i) {
        objects_[i]->Render();
    }
}


template <typename T>
void PoolAllocator<T>::Update( float elapsedSec ) {
    for(int i=0; i<(int)objects_.size(); ++i) {
        objects_[i]->Update(elapsedSec);
    }
}

template <typename T>
void PoolAllocator<T>::Release() {
    for(int i=0; i<(int)objects_.size(); ++i)
        delete objects_[i];
    objects_.clear();
}

template <typename T>
void PoolAllocator<T>::reset() {
    for(int i=0; i<(int)objects_.size(); ++i) {
        objects_[i]->reset();
    }
}
template <typename T>
void PoolAllocator<T>::PostRender() {
    for(int i=0; i<(int)objects_.size(); ++i) {
        if (objects_[i]->GetActive())
			objects_[i]->PostRender();
    }
}
*/