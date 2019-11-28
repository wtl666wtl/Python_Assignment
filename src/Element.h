#ifndef _element_h
#define _element_h

#include "Bignumber.h"
using namespace std;

class Element{
	public:
	//none 0 int 2 float 3 bool 1 str 4
	Bignumber a;double b;bool c;string s;int fl;
    Element(){
        s="";fl=0;
    }
	Element(string gs,int f=0){
		s="";
		fl=f;
		if(fl==0)s="None";
		if(fl==1)c=gs[0]=='T'?1:0;
		if(fl==2)a=gs;
		if(fl==3)b=atof(gs.c_str());
		if(fl==4)gs.erase(0,1),gs.erase(gs.length()-1,1),s=gs;
	}
	Element operator+(const Element &x)const{
		Element a=*this,b=x;
		if(a.fl>b.fl)swap(a,b);
		if(b.fl==4){
			a.s+=b.s;
		}
		if(b.fl==3){
			a.Flo();
			a.b+=b.b;
		}
		if(b.fl==2){
			a.Int();
			a.a+=b.a;
		}
		if(b.fl==1){
			a.c+=b.c;
		}
		return a;
	}
    Element operator-()const{
        Element a=*this;
        if(a.fl==3){
            a.b=-a.b;
        }
        if(a.fl==2){
            a.a=-a.a;
        }
        if(a.fl==1){
            a.c=-a.c;
        }
        return a;
    }
	Element operator-(const Element x)const{
		Element a=*this,b=x;
		if(a.fl>b.fl)swap(a,b);
		if(b.fl==3){
			a.Flo();
			a.b-=b.b;
		}
		if(b.fl==2){
			a.Int();
			a.a-=b.a;
		}
		if(b.fl==1){
			a.c-=b.c;
		}
		return a;
	}
	Element operator*(const Element x)const{
		Element a=*this,b=x;
		if(a.fl==4){
			b.Int();
			string ss=a.s;
			int tmp=(b.a).real();
			for(int i=2;i<=tmp;i++)a.s+=ss;
			return a;
		}
		if(a.fl>b.fl)swap(a,b);
		if(b.fl==3){
			a.Flo();
			a.b*=b.b;
		}
		if(b.fl==2){
			a.Int();
			a.a*=b.a;
		}
		if(b.fl==1){
			a.c*=b.c;
		}
		return a;
	}
	Element operator^(const Element x)const{
		Element a=*this,b=x;
		a.Int();b.Int();
		a.a^=b.a;
		return a;
	}
	Element operator/(const Element x)const{
		Element a=*this,b=x;
		a.Flo();b.Flo();
		a.b/=b.b;
		return a;
	}
	Element operator%(const Element x)const{
		Element a=*this,b=x;
		a.Int();b.Int();
		a.a%=b.a;
		return a;
	}
	Element operator+=(const Element x){
		return *this=*this+x;
	}
	Element operator-=(const Element x){
		return *this=*this-x;
	}
	Element operator*=(const Element x){
		return *this=*this*x;
	}
	Element operator/=(const Element x){
		return *this=*this/x;
	}
	Element operator^=(const Element x){
		return *this=*this^x;
	}
	Element operator%=(const Element x){
		return *this=*this%x;
	}
	Element &operator=(const Element x){
		a=x.a;
		b=x.b;
		c=x.c;
		s=x.s;
		fl=x.fl;
		return *this;
	}
	bool operator==(const Element x)const{
		Element a=*this,b=x;
		if(a.fl>b.fl)swap(a,b);
		if(b.fl==4){
			if(a.s==b.s)return 1;
			else return 0;
		}
		if(b.fl==3){
			a.Flo();
			if(a.b==b.b)return 1;
			else return 0;
		}
		if(b.fl==2){
			a.Int();
			if(a.a==b.a)return 1;
			else return 0;
		}
		if(b.fl==1){
			if(a.c==b.c)return 1;
			else return 0;
		}
	}
	bool operator<=(const Element x)const{
		return *this<x||*this==x;
	}
	bool operator>=(const Element x)const{
		return *this>x||*this==x;
	}
	bool operator!=(const Element x)const{
		return !(*this==x);
	}
	bool operator<(const Element x)const{
		Element a=*this,b=x;
		if(a.fl>b.fl)swap(a,b);
		if(b.fl==4){
			if(a.s<b.s)return 1;
			else return 0;
		}
		if(b.fl==3){
			a.Flo();
			if(a.b<b.b)return 1;
			else return 0;
		}
		if(b.fl==2){
			a.Int();
			if(a.a<b.a)return 1;
			else return 0;
		}
		if(b.fl==1){
			if(a.c<b.c)return 1;
			else return 0;
		}
	}
	bool operator>(const Element x)const{
		  return !(*this==x||*this<x);
	}
	Element Str(){
		ostringstream myos;
		if(fl==2)s=a.str();
		else if(fl==3)myos<<b,s=myos.str();
		else if(fl==1)c==0?s="False":"True";
		fl=4;
		return *this;
	}
	Element Int(){
		if(fl==1)a=1LL*c;
		else if(fl==3)a=1LL*b;
		fl=2;
		return *this;
	}
	Element Flo(){
		if(fl==1)b=1.0*c;
		else if(fl==2)b=1.0*a.real();
		fl=3;
		return *this;
	}
	Element Boo(){
		if(fl==2)c=a.real();
		else if(fl==3)c=(int)b;
		else if(fl==4)c=s!="";
		fl=1;
		return *this;
	}
	void print(){
		if(fl==0)printf("None");
		if(fl==1)c==1?printf("True"):printf("False");
		if(fl==2)a.print();
		if(fl==3)printf("%.6lf",b);
		if(fl==4)cout<<s;
	}
};

#endif
