#ifndef ISERIALIZABLE_HPP
#define ISERIALIZABLE_HPP


#include <string>

template <typename T>
class ISerializable {
    public:
        virtual ~ISerializable( void ) {}
        virtual std::string serialize( T ) = 0 {};
        virtual T deserialize( std::string ) = 0 {};
};


#endif // ISERIALIZABLE_HPP
