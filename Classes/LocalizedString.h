#include "cocos2d.h"

#define LHLocalizedString(__KEY__) LocalizedString::localizedString(__KEY__)
#define LHLocalizedCString(__KEY__) LocalizedString::localizedString(__KEY__)->getCString()

class LocalizedString {
    
public:
    LocalizedString();
    
    virtual ~LocalizedString();
    
    static cocos2d::__String* localizedString(const char *key);
    
private:
    static const char* getSystemLang();
    
};