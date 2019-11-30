#ifndef _bignumber_h
#define _bignumber_h

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

class Bignumber;
bool bj(int l,int r,const Bignumber& d,const Bignumber& b);

class Bignumber {
	public:
	int w,neg;vector<int>x;
    
    Bignumber()
    {
        w=0;neg=1;x.clear();
    }
	
    Bignumber(ll a)
    {
        *this=a;
    }
    
    Bignumber(string s)
    {
        *this=s;
    }

    Bignumber operator=(ll a)
    {
    	x.clear();
    	if(a==0){neg=1;w=1;x.push_back(0);}
    	if(a<0)neg=-1,a=-a;else neg=1;
		w=0;
        for (;a;a/=10,w++)x.push_back(a%10);
        return *this;
    }

    Bignumber operator=(string s)
    {
    	x.clear();
        w=0;neg=(s[0]=='-'?-1:1);
        int n=s.length();
        for(int i=n-1;i>=(neg==1?0:1);i--,++w){
        	int tmp=s[i]-'0';
            x.push_back(tmp);
        }
        return *this;
    }

    Bignumber operator+(const Bignumber& b) const
    {
    	if(neg==-1&&b.neg==1){
    		return b-(-*this);
		}else if(neg==-1&&b.neg==-1){
			return -((-*this)+(-b));
		}else if(neg==1&&b.neg==-1){
			if(*this>(-b)){
				Bignumber c;
				int f=0,tmp;c.w=w;
    	    	for(int i=0;i<c.w;++i){
       		 		tmp=x[i]-(b.w>i?b.x[i]:0)+f;
       		 		if(tmp<0)tmp+=10,f=-1;else f=0;
       		 		(c.x).push_back(tmp);
				}
				while(c.x[c.w-1]==0&&c.w>1)c.w--;
        		return c;
			}else return -((-b)-*this);
		}
        Bignumber c;
        int f=0,tmp;c.w=max(w,b.w);
        for(int i=0;i<c.w;++i){
        	tmp=(w>i?x[i]:0)+(b.w>i?b.x[i]:0)+f;
        	if(tmp>9)f=1,tmp-=10,(c.x).push_back(tmp);
            else f=0,(c.x).push_back(tmp);
		}
		if(f)c.w++,(c.x).push_back(1);
        return c;
    }

    Bignumber operator+=(const Bignumber& b)
    {
        return *this=*this+b;
    }

    Bignumber operator-() const
    {
        Bignumber c=*this;c.neg=-c.neg;
        return c;
    }

    Bignumber operator-(const Bignumber& b) const
    {
        return *this+(-b);
    }

    Bignumber operator-=(const Bignumber& b)
    {
        return *this=*this-b;
    }

    Bignumber operator*(const Bignumber& b) const
    {
        Bignumber c;
        c.neg=neg*b.neg;
        for(int i=0;i<b.w;++i){
        	int f=0;
            for(int j=0;j<w;++j){
                ll t=(ll)b.x[i]*x[j]+(c.w>j+i?c.x[j+i]:0)+f;
                if(j+i>=c.w)c.w++,(c.x).push_back(t%10);
                else c.x[j+i]=t%10;
                f=t/10;
            }
            if(f>0){
            	if(i+w>=c.w)c.w++,(c.x).push_back(f);
            	else c.x[i+w]+=f;
			}
        }
        return c;
    }

    Bignumber operator*=(const Bignumber& b)
    {
        return *this=*this*b;
    }
    
    Bignumber abs(const Bignumber& b) const
    {
    	Bignumber c=b<0LL?-b:b;
        return c;
    }
    
    // ^ -> //
    Bignumber operator^(const Bignumber& b) const
    {
        Bignumber c;
		Bignumber d=(*this);
        c.neg=neg*b.neg;
        if(abs(d)<abs(b)){
        	c.w=1;
        	if(c.neg==1)(c.x).push_back(0);
        	else (c.x).push_back(1);
        	return c;
		}
        c.neg=neg*b.neg;
        for(int s=w-1,t=w-b.w;t>=0;t=(s-b.w+1==t?s-b.w:s-b.w+1)){
        	while(!bj(s,t,d,b)){
        		t--;
        		if(s!=w-1)c.w++,(c.x).push_back(0);
			}
        	int tmp=0;
        	while(bj(s,t,d,b)){
        		for(int i=t,j=0;i<=s;j++,i++){
					if(j<b.w)d.x[i]-=b.x[j];
					if(d.x[i]<0)d.x[i]+=10,d.x[i+1]--;
				}
				tmp++;
        		while(d.x[s]==0&&s>=0)s--;
			}
			c.w++;(c.x).push_back(tmp);
			for(int j=max(0,s-b.w+1);j<t-1;j++)c.w++,(c.x).push_back(0);
		}
		for(int i=0;i<c.w/2;i++)swap(c.x[i],c.x[c.w-i-1]);
		c.print();
		if(c.neg==-1){
		    if(c*b!=*this)c-=1ll;
		}
        return c;
    }

	// ^= -> //=
    Bignumber operator^=(const Bignumber& b)
    {
        return *this=*this^b;
    }
    
    Bignumber operator%(const Bignumber& b) const
    {
    	//Bignumber c=abs(*this)-(abs(*this)^abs(b))*abs(b);
    	//c.neg=neg;
        Bignumber c=*this-(*this^b)*b;
        return c;
    }

    Bignumber operator%=(const Bignumber& b)
    {
        return *this=*this%b;
    }

    bool operator<(const Bignumber& b) const
    {
    	if(neg>0&&b.neg<0)return 0;
    	if(neg<0&&b.neg>0)return 1;
    	if(neg<0&&b.neg<0)return (-b)<(-*this);
    	if(w<b.w)return 1;
    	if(w>b.w)return 0;
    	for(int i=w-1;i>=0;i--)
			if(x[i]>b.x[i])return 0;
			else if(x[i]<b.x[i])return 1;
		return 0;
    }

    bool operator==(const Bignumber& b) const
    {
    	if(neg!=b.neg)return 0;
    	if(w!=b.w)return 0;
    	for(int i=w-1;i>=0;i--)
			if(x[i]!=b.x[i])return 0;
		return 1;
    }
    
    bool operator>(const Bignumber& b) const
    {
    	return !(*this==b||*this<b);
    }
    
    Bignumber &operator=(const Bignumber& b)
    {
    	x.clear();
    	w=b.w;neg=b.neg;
    	for(int i=0;i<w;i++)x.push_back(b.x[i]);
        return *this;
    }
    
    bool operator<=(const Bignumber& b) const
    {
    	return *this==b||*this<b;
    }
    
    bool operator>=(const Bignumber& b) const
    {
    	return *this==b||*this>b;
    }
    
    bool operator!=(const Bignumber& b) const
    {
    	return !(*this==b);
    }
    
    void print() const
    {
        Bignumber c=*this;
        if(neg<0)putchar('-');
        int i=w-1;
		while(i>=0)printf("%d",x[i]),i--;//puts("");
    }
    
    double real() const
    {
    	double tmp=0;
        for(int i=w-1;i>=0;i--)tmp=10*tmp+x[i];
        return tmp*neg;
    }
    
    string str() const
    {
    	string tmp="";
        for(int i=w-1;i>=0;i--)tmp+=char(x[i]+48);
        if(neg==-1)tmp="-"+tmp;
        return tmp;
    }
};

    bool bj(int l,int r,const Bignumber& d,const Bignumber& b)
    {
		if(l-r+1<b.w)return 0;
		if(l-r+1>b.w)return 1;
		for(int i=l,j=b.w-1;i>=r;i--,j--)
			if(d.x[i]>b.x[j])return 1;
			else if(d.x[i]<b.x[j])return 0;
		return 1;
	}

#endif
