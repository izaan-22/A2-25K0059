#ifndef USER_H
#define USER_H

class user {
public:
    virtual void showPanel() = 0;
    virtual void showProfile() const = 0;
    virtual ~user() {}
};

#endif