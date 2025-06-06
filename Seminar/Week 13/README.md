# Dynamic Memory Management

<slidebreak/>

### RAII (Resource Acquisition Is Initialization)

- Динамичната памет се освобождава от този, който я е заделил.
- RAII е принцип, при който ресурсите най-общо казано се придобиват в конструктора и се освобождават в деструктора - тоест са обвързани с жизнения цикъл на обекта - този, който е заделил ресурса, той ще го освободи.
- Тук е и приложението на голямата четворка (шестица).

<slidebreak/>

### Smart Pointers

- Не винаги задалената динамична памет се ползва само от един обект.
- В такива случаи, ако достъпваните обекти могат да бъдат самостоятелно съществуващи и е смислено да съществуват в контекст без източника си, може да се направи копие, но това е скъпа операция.
- В случаи, в които достъпваният обект се използва от множество други обекти/функции/..., е по-добре да се използват умни указатели.
- Умните указатели са класове, които автоматично освобождават ресурсите, когато вече не са необходими, тоест когато вече никой не ги използва.
- Предимство на умните указатели е, че е ясно кой се грижи за освобождаването на паметта при по-сложни сценарии.
- Но се нарушава RAII принципа, защото ресурсът не се освобождава в деструктора на обекта, който го е заделил, а в деструктора на умния указател.

<slidebreak/>

Има няколко вида умни указатели в C++:

- Unique pointer (`std::unique_ptr`): Указател, който притежава единствено указания обект. Когато `std::unique_ptr` излезе от обхвата си, паметта, която е заделил, се освобождава автоматично.
- Shared pointer (`std::shared_ptr`): Указател, който може да бъде споделян между няколко инстанции. Паметта се освобождава автоматично, когато последният `std::shared_ptr`, който притежава обекта, излезе от обхвата си.
- Weak pointer (`std::weak_ptr`): Указател, който не притежава обекта, а само наблюдава `std::shared_ptr`. Той не увеличава броя на споделените указатели и не предотвратява освобождаването на паметта.

<slidebreak/>

### UniquePtr

- Wrapper за обикновен указател, който не може да се копира - тоест съществува като единствен екземпляр по памет, която ще бъде унищожена, когато указателят излезе от обхвата си.
- Ако искаме да го подаване на функции, може единстевно като референция, но не и като стойност.
- Този указател не може да бъде копиран, но може да бъде преместван, чрез използване на move семантиката.
  - Move sematics позволява прехвърляне на ресурси от един обект на друг, без да се прави копие на данните, а те се преместват, инвалидирайки обекта, от който са преместени.
  - Това е полезно, когато искаме да избегнем ненужни копия на големи обекти или ресурси, които вече са непотребни (тоест ще бъдат унищожени).
  - Това се прилага за "rvalue референции" (`&&`), които се създават чрез оператора `std::move(<object>)` изкуствено или ако обектът си е rvalue може да бъде подаден на такава референция.
  - Важна стъпка е инвалидиране на оригиналния обект, от който се преместват ресурсите, за да не се получи двойно освобождаване на паметта.
  - Move семантиката се прилага за копиращия конструктор и копиращия оператор за присвояване, които трябва да са noexcept.
  - Move семантиката допълва голямата четворка до голяма шестица.

<slidebreak/>

```cpp
template <typename T>
class UniquePtr {
    T* ptr;

public:

    UniquePtr(T* p = nullptr) : ptr(p) {}

    ~UniquePtr() noexcept {
        delete ptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    T* get() const { return ptr; }
};
```

<slidebreak/>

### SharedPtr

- Wrapper за обикновен указател, който може да бъде споделян между няколко инстанции.
- За разлика от `UniquePtr`, `SharedPtr` използва брояч на референции, който следи колко указателя сочат към същия обект.
- Когато направим копие на `SharedPtr`, броячът се увеличава, а когато `SharedPtr` излезе от обхвата си, броячът се намалява.
- Когато броячът достигне нула, паметта, която е заделена, се освобождава автоматично. Тоест указателят се грижи за освобождаването на паметта, чак когато вече никой не я използва.

```cpp
template <typename T>
class SharedPtr {
    T* ptr;
    size_t* refs;

public:
    SharedPtr(T* p = nullptr) : ptr(p), refs(nullptr) {
        if(ptr) {
            refs = new size_t(1);
        }
    }

    ~SharedPtr() noexcept {
        release();
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), refs(other.refs) {
        addRef();
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refs = other.refs;
            addRef();
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    size_t use_count() const { return refs ? *refs : 0; }

private:
void release() noexcept {
        if(!ptr) return;

        if (--(*refs) == 0) {
            delete ptr;
            delete refs;
            ptr = nullptr;
            refs = nullptr;
        }
    }

    void addRef() {
        if (ptr && refs) {
            ++(*refs);
        }
    }
};
```

<slidebreak/>

### WeakPtr

- `WeakPtr` е специален вид указател, който надгражда `SharedPtr`.
- Той не увеличава броя на споделените указатели и не предотвратява освобождаването на паметта.
- `WeakPtr` се използва, когато искаме да наблюдаваме обект, който е притежаван от `SharedPtr`, без да увеличаваме броя на споделените указатели.
- Това е полезно, когато искаме да избегнем циклични зависимости между обекти, които могат да доведат до утечки на памет.

```cpp
template <typename T>
class SharedPtr {
    T* ptr;
    size_t* refs;
    size_t* weaks;

    SharedPtr(T* p, size_t* r, size_t* w) : ptr(p), refs(r), weaks(w) {
        addRef();
    }

public:
	template <typename T>
	friend class WeakPtr;

    SharedPtr(T* p = nullptr) : ptr(p), refs(nullptr), weaks(nullptr) {
        if(ptr) {
            refs = new size_t(1);
            weaks = new size_t(0);
        }
    }

    ~SharedPtr() noexcept {
        release();
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), refs(other.refs), weaks(other.weaks) {
        addRef();
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refs = other.refs;
            weaks = other.weaks;
            addRef();
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    operator bool() const {
        return ptr != nullptr;
    }

    size_t use_count() const { return refs ? *refs : 0; }
    size_t weak_count() const { return weaks ? *weaks : 0; }

private:
    void release() noexcept {
        if(!ptr) return;

        if (--(*refs) == 0) {
            delete ptr;
            ptr = nullptr;
        }

        if(*weaks == 0) {
            delete refs;
            refs = nullptr;
            delete weaks;
            weaks = nullptr;
        }
    }

    void addRef() {
        if (ptr && refs) {
            ++(*refs);
        }
    }
};

template <typename T>
class WeakPtr {
    T* ptr;
    size_t* refs;
    size_t* weaks;

public:
    WeakPtr(const SharedPtr<T>& shared) : ptr(shared.ptr), refs(shared.refs), weaks(shared.weaks) {
        addRef();
    }

    ~WeakPtr() noexcept {
        relase();
    }

    WeakPtr(const WeakPtr& other) : ptr(other.ptr), refs(other.refs), weaks(other.weaks) {
        addRef();
    }

    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refs = other.refs;
            weaks = other.weaks;
            addRef();
        }
        return *this;
    }

    bool expired() const {
        return !refs || *refs == 0;
    }

    SharedPtr<T> lock() const {
        if (expired()) {
            return SharedPtr<T>();
        }

        return SharedPtr<T>(ptr, refs, weaks);
    }

    operator bool() const {
        return !expired();
    }

private:
    void release() noexcept {
        if (weaks && --(*weaks) == 0 && refs && *refs == 0) {
            delete weaks;
            weaks = nullptr;
            delete refs;
            refs = nullptr;
        }
    }

    void addRef() {
        if (weaks) {
            ++(*weaks);
        }
    }
};
```

<slidebreak/>

### Как се ползва

- Ако притежаваме обекта => SharedPtr
- Ако наблюдаваме обекта => WeakPtr

```cpp

class Child;

class Parent {
    SharedPtr<Child> child;

public:
    Parent() {}

    void setChild(SharedPtr<Child> c) {
        child = c;
    }

    void doSomething() {
        // child->....
    }

    const SharedPtr<Child>& getChild() {
        return child;
    }
}

class Child {
    WeakPtr<Parent> parent;

public:
    Child() {}

    void setParent(const SharedPtr<Parent>& p) {
        parent = p;
    }

    void doSomething() {
        if (!parent) {
            throw std::logic_error("Parent no longer exists");
        } 
        
        // Use the weak pointer on demand -> 
        // the created shared ptr will be destroyed when exiting the function scope
        SharedPtr<Parent> parentPtr = parent.lock();
        // parentPtr-> .... (possible danger of recursion!)
    }
};


int main()
{
    SharedPtr<Parent> parent = new Parent();
    SharedPtr<Child> child = new Child();

    // 2-way relationship
    parent->setChild(child);
    child->setParent(parent);

    parent->doSomething(); // Calls the child's methods using the shared pointer

    SharedPtr<Child> child = parent->getChild();
    child->doSomething(); // Calls the parent's methods using the weak -> shared pointer

    // When parent goes out of scope, both Parent and Child will be destroyed
    return 0;
}
```

- Тези правила позволяват избягването на циклични зависимости и така указателите се унищожават правилно.
- Удобни при работа в контекста на композиция и агрегация.
