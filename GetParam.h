#include <string>


#ifndef GetParam_H
#define	GetParam_H

class GetParam {
public:
    GetParam();
    virtual ~GetParam();
    void showMenu(void);
private:
    WINDOW * m_window;
    static const std::string m_GetParamText []; 
};

#endif	/* GetParam_H */

