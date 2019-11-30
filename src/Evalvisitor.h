#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#define fi first
#define se second
#include "Element.h"
#include "Python3BaseVisitor.h"
using namespace std;
int cnt=0;//cengshu
int numf=0,func=0;//functions
int id=0;//continue=1 break=2 or return=3
map<string , Element >g[2006];
map<string , int>nf;
map<string , Element >ff[200006];
vector<string>vf[200006];
Python3Parser::SuiteContext *lis[200006];

class EvalVisitor: public Python3BaseVisitor {
public:
    virtual antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
        numf=5;
        nf["print"]=1;
        nf["int"]=2;
        nf["float"]=3;
        nf["str"]=4;
        nf["bool"]=5;
        for(const auto &c : ctx->stmt()){
            visitStmt(c);
        }
    }

    virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        nf[ctx->NAME()->getText()]=++numf;
        visitParameters(ctx->parameters());
        lis[numf]=ctx->suite();
        return ctx;
    }

    virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        if(ctx->typedargslist()!= NULL)visitTypedargslist(ctx->typedargslist());
        return ctx;
    }

    virtual antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        int j=ctx->test().size()-1,i=ctx->tfpdef().size()-1;
        for(;j>=0;i--,j--){
            string ss = visitTfpdef(ctx->tfpdef()[i]);
            Element tmp = visitTest(ctx->test()[j]);
            ff[numf][ss] = tmp;
            vf[numf].push_back(ss);
        }
        for(;i>=0;i--){
            string ss = visitTfpdef(ctx->tfpdef()[i]);
            Element tmp("", 0);
            ff[numf][ss] = tmp;
            vf[numf].push_back(ss);
        }
        return ctx;
    }

    virtual antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
        return ctx->NAME()->getText();
    }

    virtual antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        if(ctx->simple_stmt()==NULL)return visitCompound_stmt(ctx->compound_stmt());
        else return visitSimple_stmt(ctx->simple_stmt());
    }

    virtual antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        if(ctx->small_stmt()!=NULL)return visitSmall_stmt(ctx->small_stmt());
    }

    virtual antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        if(ctx->expr_stmt()!=NULL)return visitExpr_stmt(ctx->expr_stmt());
        else return visitFlow_stmt(ctx->flow_stmt());
    }

    virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        //return visitTestlist(ctx->testlist()[0]);
        string sss=ctx->getText();
        int siz=ctx->testlist().size();
        vector<Element>tmp=visitTestlist(ctx->testlist()[siz-1]);
        if(ctx->augassign()!=NULL){
            string op=visitAugassign(ctx->augassign());
            for(int i=0;i<tmp.size();i++){
                string ss=ctx->testlist()[0]->test()[i]->getText();
                if(op=="+="){
                    g[cnt][ss]+=tmp[i];
                }else if(op=="-="){
                    g[cnt][ss]-=tmp[i];
                }else if(op=="*="){
                    g[cnt][ss]*=tmp[i];
                }else if(op=="/="){
                    g[cnt][ss]/=tmp[i];
                }else if(op=="//="){
                    g[cnt][ss]^=tmp[i];
                }else if(op=="%=") {
                    g[cnt][ss]%= tmp[i];
                }
            }
        }else{
            for(int k=0;k<siz-1;k++){
                for(int i=0;i<tmp.size();i++){
                    string ss=ctx->testlist()[k]->test()[i]->getText();
                    g[cnt][ss]=tmp[i];
                    //cout<<"TEST:"<<cnt<<' '<<ss<<' ';g[cnt][ss].print();puts("");
                }
            }
        }
        vector<Element>t1;t1.clear();
        return t1;
    }

    virtual antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
        return ctx->getText();
    }

    virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        if(ctx->break_stmt()!=NULL){
            return visitBreak_stmt(ctx->break_stmt());
        }else if(ctx->continue_stmt()!=NULL){
            return visitContinue_stmt(ctx->continue_stmt());
        }else{
            return visitReturn_stmt(ctx->return_stmt());
        }
    }

    virtual antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        id=2;
        vector<Element>t1;t1.clear();
        return t1;
    }

    virtual antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
        id=1;
        vector<Element>t1;t1.clear();
        return t1;
    }

    virtual antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
        id=3;
        int funct=func;
        if(ctx->testlist()!=NULL) {
            vector<Element> tmp = visit(ctx->testlist());
            return tmp;
        }
        vector<Element>t1;t1.clear();
        return t1;
    }

    virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        if(ctx->if_stmt()!=NULL){
            visitIf_stmt(ctx->if_stmt());
        }else if(ctx->while_stmt()){
            visitWhile_stmt(ctx->while_stmt());
        }else{
            visitFuncdef(ctx->funcdef());
        }
        vector<Element>t1;t1.clear();
        return t1;
    }

    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        vector<Element>tmp=visitTest(ctx->test()[0]);
        int pos=0;tmp[0].Boo();
        while(tmp[0].c!=1&&pos+1<ctx->test().size()){
            pos++;
            vector<Element>tmp1=visitTest(ctx->test()[pos]);
            tmp1[0].Boo();tmp=tmp1;
        }
        if(tmp[0].c==1)visit(ctx->suite()[pos]);
        else if(pos+1<ctx->suite().size())visit(ctx->suite()[pos+1]);
        vector<Element>t1;t1.clear();
        return t1;
    }

    virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        vector<Element>tmp=visitTest(ctx->test());
        tmp[0].Boo();int orz=0;id=0;
        while(tmp[0].c==1){
            visit(ctx->suite());
            if(id==2){id=0;break;}
            else if(id==1)id=0;
            else if(id==3)break;
            vector<Element>tmp1=visitTest(ctx->test());
            tmp1[0].Boo();tmp=tmp1;
        }
        vector<Element>t1;t1.clear();
        return t1;
    }

    virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        if(ctx->simple_stmt())visitSimple_stmt(ctx->simple_stmt());
        for(int i=0;i<ctx->stmt().size();i++) {
            vector<Element>tmp=visitStmt(ctx->stmt()[i]);
            if(id==1||id==2)break;
            if(id==3)return tmp;
        }
        vector<Element>t1;t1.clear();
        return t1;
        //return visitStmt(ctx->stmt()[ctx->stmt().size()-1]);
    }

    virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        return visitOr_test(ctx->or_test());
    }

    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        vector<Element>key=visitAnd_test(ctx->and_test()[0]);
        if(ctx->and_test().size()==1)return key;
        else{
            key[0].Boo();
        }
        for(int i=1;i<ctx->and_test().size();i++){
            vector<Element>tmp=visitAnd_test(ctx->and_test()[i]);
            if(tmp[0].c==1)key[0].c=1;
        }
        return key;
    }

    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        vector<Element>key=visitNot_test(ctx->not_test()[0]);
        if(ctx->not_test().size()==1)return key;
        else{
            key[0].Boo();
        }
        for(int i=1;i<ctx->not_test().size();i++){
            vector<Element>tmp=visitNot_test(ctx->not_test()[i]);
            if(tmp[0].c==0)key[0].c=0;
        }
        return key;
    }

    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        if(ctx->not_test()!=NULL){
            vector<Element>key=visitNot_test(ctx->not_test());
            key[0].Boo();
            if(key[0].c==0)key[0].c=1;else key[0].c=0;
            return key;
        }else{
            return visitComparison(ctx->comparison());
        }
    }

    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        vector<Element>key=visitArith_expr(ctx->arith_expr()[0]),tmp=key;
        if(ctx->comp_op().size()>0){
            key[0].Boo();
            key[0].c=1;
        }
        for(int i=0;i<ctx->comp_op().size();i++){
            string ss=visitComp_op(ctx->comp_op()[i]);
            vector<Element>tmp2=visitArith_expr(ctx->arith_expr()[1+i]);
            if(ss=="<"){
                key[0].c&=tmp[0]<tmp2[0];
            }else if(ss==">"){
                key[0].c&=tmp[0]>tmp2[0];
            }else if(ss=="=="){
                key[0].c&=tmp[0]==tmp2[0];
            }else if(ss==">="){
                key[0].c&=tmp[0]>=tmp2[0];
            }else if(ss=="<="){
                key[0].c&=tmp[0]<=tmp2[0];
            }else if(ss=="!="){
                key[0].c&=tmp[0]!=tmp2[0];
            }
            tmp=tmp2;
        }
        return key;
    }

    virtual antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
        return ctx->getText();
    }

    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        vector<Element>key=visitTerm(ctx->term()[0]);
        string ss=ctx->getText();
        //cout<<ss<<endl;
        ss.erase(0,ctx->term()[0]->getText().length());
        for(int i=1;i<ctx->term().size();i++){
            vector<Element>tmp=visitTerm(ctx->term()[i]);
            if(ss[0]=='+'){
                key[0]+=tmp[0];
            }else{
                key[0]-=tmp[0];
            }
            ss.erase(0,1+ctx->term()[i]->getText().length());
        }
        return key;
    }

    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        vector<Element>key=visitFactor(ctx->factor()[0]);
        string ss=ctx->getText();
        ss.erase(0,ctx->factor()[0]->getText().length());
        for(int i=1;i<ctx->factor().size();i++){
            int p=1;
            vector<Element>tmp=visitFactor(ctx->factor()[i]);
            if(ss[0]=='*'){
                key[0]*=tmp[0];
            }else if(ss[0]=='%'){
                key[0]%=tmp[0];
            }else if(ss[0]=='/'&&ss[1]!='/'){
                key[0]/=tmp[0];
            }else{
                p++;
                key[0]^=tmp[0];
            }
            ss.erase(0,p+ctx->factor()[i]->getText().length());
        }
        return key;
    }

    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        if(ctx->factor()!=NULL){
            string ss=ctx->getText();
            vector<Element>key=visitFactor(ctx->factor());
            if(ss[0]=='-')key[0]=-key[0];
            return key;
        }
        else return visitAtom_expr(ctx->atom_expr());
    }

    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        //cout<<ctx->getText()<<endl;
        if(ctx->trailer()!=NULL){
            if(ctx->atom()->NAME()){
                //visitAtom(ctx->atom());
                int funct=func=nf[ctx->atom()->NAME()->getText()];
                if(funct<=5){
                    Element t1=visitTrailer(ctx->trailer());
                    vector<Element>tmp;tmp.clear();
                    tmp.push_back(t1);func=0;
                    return tmp;
                }else{
                    cnt++;
                    for(int i=0;i<vf[funct].size();i++){
                            auto p=vf[funct][i];
                            if(ff[funct][p].fl!=0)g[cnt][p]=ff[funct][p];
                        }
                    visitTrailer(ctx->trailer());
                    vector<Element>tmp=visitSuite(lis[funct]);
                    id=0;
                    cnt--;func=0;
                    return tmp;
                }
            }
        }else{
            vector<Element>tmp;tmp.clear();
            tmp.push_back(visitAtom(ctx->atom()));
            return tmp;
        }
    }

    virtual antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        if(ctx->arglist()!=NULL)return visitArglist(ctx->arglist());
        return ctx;
    }

    virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        //cout<<ctx->getText()<<endl;
        //if(ctx->STRING().size())cout<<ctx->STRING()[0]->getText()<<endl;
        if(ctx->NAME()!=NULL){
            for(int i=cnt;;i--)
                if(g[i].count(ctx->NAME()->getText()))
                    return g[i][ctx->NAME()->getText()];
        }else if(ctx->test()!=NULL){
            return visitTest(ctx->test());
        }else if(ctx->NUMBER()!=NULL){
            string ss=ctx->NUMBER()->getText();
            Element tmp(ss,2);
            if(ss.find('.')<ss.length())tmp=Element(ss,3);
            return tmp;
        }else if(ctx->STRING().size()){
            Element tmp(ctx->STRING()[0]->getText(), 4);
            return tmp;
        }else if(ctx->getText()=="None"){
            return Element("",0);
        }else return Element(ctx->getText(),1);
    }

    virtual antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
        vector<Element>tmp;tmp.clear();
        for(const auto &c : ctx->test()){
            vector<Element>orz=visitTest(c);
            for(int i=0;i<orz.size();i++)
                tmp.push_back(orz[i]);
        }
        return tmp;
    }

    virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        int funct=func;
        Element tmp=visitArgument(ctx->argument()[0]);
        if(funct>5&&ctx->argument()[0]->NAME()==NULL)
            g[cnt][vf[funct][0]]=tmp;
        for(int i=1;i<ctx->argument().size();i++){
            if(funct==1)putchar(' ');
            tmp=visitArgument(ctx->argument()[i]);
            if(funct!=1&&ctx->argument()[0]->NAME()==NULL)g[cnt][vf[funct][i]]=tmp;
        }
        if(funct==1)puts("");
        else if(funct==2)tmp.Int();
        else if(funct==3)tmp.Flo();
        else if(funct==4)tmp.Str();
        else if(funct==5)tmp.Boo();
        return tmp;
    }

    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        Element key("",0);
        int funct=func;
        if(ctx->NAME()!=NULL) {
            const auto &c=ctx->NAME();
            g[cnt][c->getText()]=visitTest(ctx->test());
            key=g[cnt][c->getText()];
        }else{
            vector<Element>tmp=visitTest(ctx->test());
            key=tmp[0];
            //printf("test:");tmp[0].print();puts("");
        }
        if(funct==1)key.print();
        return key;
    }
};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H
