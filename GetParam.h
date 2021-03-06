//    File: GetParam.h
//    Copyright (C) 2015  Patrik Faistaver
//    
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

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

