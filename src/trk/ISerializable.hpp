#ifndef ISERIALIZABLE_HPP
#define ISERIALIZABLE_HPP

template <typename T>
class ISerializable {
  public:
    virtual ~ISerializable() = 0;
    virtual T serialize() const = 0;
    virtual void deserialize(T const& data) = 0;
};

// Even pure virtual destructor should have an implementation
template <typename T>
inline ISerializable<T>::~ISerializable() {}

#endif // !ISERIALIZABLE_HPP
