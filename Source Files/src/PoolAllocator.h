#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <vector>

template <typename T>
class PoolAllocator {
    public:
        PoolAllocator<T>(int totalObjects):totalObjects_(totalObjects) {
			// for some reason the next line is giving an error => 'MPOD_ASSERT': identifier not found
            //MPOD_ASSERT(objects_.empty());
            objects_.insert(objects_.begin(), totalObjects_, nullptr);

            for(int i=0; i<totalObjects_; ++i) {
                objects_[i]=new T();
            }
        };
        ~PoolAllocator<T>() {
            Release();
        };

        virtual void Initialise()=0;

        //tidy up on way out
        void Release() {
            for(int i=0; i<(int)objects_.size(); ++i)
                delete objects_[i];
            objects_.clear();
        };

        void Update(float elapsedSec) {
            for(int i=0; i<(int)objects_.size(); ++i) {
                objects_[i]->Update(elapsedSec);
            }
        };

        void Render() {
            //SetupMaterial(1,1,1);
            for(int i=0; i<(int)objects_.size(); ++i) {
                objects_[i]->Render();
            }
        };

        //sprites, additive effects, etc
        void PostRender() {
            for(int i=0; i<(int)objects_.size(); ++i) {
                if (objects_[i]->GetActive())
                    objects_[i]->PostRender();
            }
        };

        void reset() {
            for(int i=0; i<(int)objects_.size(); ++i) {
                objects_[i]->reset();
            }
        };

        int getTotalObjects() {
            return totalObjects_;
        };

        std::vector<T*> &getObjects() {
            return objects_;
        };

    protected:
        // total objects held in array
        int totalObjects_;
        //array of all objects
        std::vector<T*> objects_;
};


#endif