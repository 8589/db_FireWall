#include <memory>
#include <iostream>

class parent
{
public:
	parent(){};
	virtual ~parent(){};

	virtual void f(){
		std::cout << "parent" << std::endl;
	}
	
};


class child : public parent
{
public:
	child(){};
	virtual	~child(){};
	
	void f()
	{
		std::cout << "child" << std::endl;
	}
};


void test(std::shared_ptr<parent> p)
{
	p->f();
}



class tt
{
private:
	std::shared_ptr<parent> p;
public:
	tt(std::shared_ptr<parent> _p):p(_p){};
	~tt(){};
	void f(){p->f();}
	
};

int main()
{
	test(std::shared_ptr<parent>(new child));
	tt t(std::shared_ptr<parent>(new child));
	t.f();
}