#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#define fi first
#define se second
#include "Element.h"
#include "Python3BaseVisitor.h"
using namespace std;
int cnt=0;//cengshu
int numf=0,func=0;//functions
map<string , Element >g[2006];
map<string , int>nf;
map<string , Element >ff[200006];
vector<string >vf[200006];
vector<int> list;

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
        visitSuite(ctx->suite());
    }

    virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        if(ctx->typedargslist()!= NULL)visitTypedargslist(ctx->typedargslist());
    }

    virtual antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        int oo=0;
        for(const auto &c : ctx->tfpdef()){
            string ss=visitTfpdef(c);
            //ctx->children[1];
            if(ctx->test()[oo]!=NULL) {
                Element tmp=visitTest(ctx->test()[oo]);
                ff[numf][ss]=tmp;
                vf[numf].push_back(ss);
            }else{
                Element tmp("",0);
                ff[numf][ss]=tmp;
                vf[numf].push_back(ss);
            }
            oo++;
        }
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
        if(ctx->augassign()){
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
        return tmp;
    }

    virtual antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
        return ctx->getText();
    }

    virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        if(ctx->break_stmt()!=NULL){
            return visitBreak_stmt(ctx->break_stmt());
        }if(ctx->continue_stmt()!=NULL){
            return visitContinue_stmt(ctx->continue_stmt());
        }else{
            return visitReturn_stmt(ctx->return_stmt());
        }
    }

    virtual antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        cnt--;
        //...
    }

    virtual antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
    }

    virtual antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    }

    virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        if(ctx->if_stmt()!=NULL){
            visitIf_stmt(ctx->if_stmt());
        }else if(ctx->while_stmt()){
            visitWhile_stmt(ctx->while_stmt());
        }else{
            visitFuncdef(ctx->funcdef());
        }
    }

    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        Element tmp=visitTest(ctx->test()[0]);
        tmp.Boo();int s=0;
        while(tmp.c!=1){
            //do
        }
        //...do s
    }

    virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {

    }

    virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        return visitOr_test(ctx->or_test());
    }

    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        Element key("",0),tmp("",0);
        const auto &c=ctx->and_test();
        if(c.size()==1)return visitAnd_test(c[0]);
        else{
            key=visitAnd_test(c[0]);
            key.Boo();
        }
        for(int i=1;i<c.size();i++){
            tmp=visitAnd_test(c[i]);
            if(tmp.c==1)key.c=1;
        }
        puts("This is a test:");key.print();
        return key;
    }

    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        Element key("",0),tmp("",0);
        const auto &c=ctx->not_test();
        if(c.size()==1)return visitNot_test(c[0]);
        else{
            key=visitNot_test(c[0]);
            key.Boo();
        }
        for(int i=1;i<c.size();i++){
            tmp=visitNot_test(c[i]);
            if(tmp.c==0)key.c=0;
        }
        return key;
    }

    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        if(ctx->not_test()!=NULL){
            Element key=visitNot_test(ctx->not_test());
            key.Boo();
            if(key.c==0)key.c=1;else key.c=0;
            return key;
        }else{
            return visitComparison(ctx->comparison());
        }
    }

    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        Element key=visitArith_expr(ctx->arith_expr()[0]),tmp=key;
        if(ctx->comp_op().size()>0){
            key.Boo();
            key.c=1;
        }
        for(int i=0;i<ctx->comp_op().size();i++){
            string ss=visitComp_op(ctx->comp_op()[i]);
            Element tmp2=visitArith_expr(ctx->arith_expr()[1+i]);
            if(ss=="<"){
                key.c&=tmp<tmp2;
            }else if(ss==">"){
                key.c&=tmp>tmp2;
            }else if(ss=="=="){
                key.c&=tmp==tmp2;
            }else if(ss==">="){
                key.c&=tmp>=tmp2;
            }else if(ss=="<="){
                key.c&=tmp<=tmp2;
            }else if(ss=="!="){
                key.c&=tmp!=tmp2;
            }
            tmp=tmp2;
        }
        return key;
    }

    virtual antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
        return ctx->getText();
    }

    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        Element key=visitTerm(ctx->term()[0]);
        string ss=ctx->getText();
        //cout<<ss<<endl;
        ss.erase(0,ctx->term()[0]->getText().length());
        for(int i=1;i<ctx->term().size();i++){
            if(ss[0]=='+'){
                key+=visitTerm(ctx->term()[i]);
            }else{
                key-=visitTerm(ctx->term()[i]);
            }
            ss.erase(0,1+ctx->term()[i]->getText().length());
        }
        return key;
    }

    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        Element key=visitFactor(ctx->factor()[0]);
        string ss=ctx->getText();
        ss.erase(0,ctx->factor()[0]->getText().length());
        for(int i=1;i<ctx->factor().size();i++){
            int p=0;
            if(ss[0]=='*'){
                key*=visitFactor(ctx->factor()[i]);
            }else if(ss[0]=='%'){
                key%=visitFactor(ctx->factor()[i]);
            }else if(ss[0]=='/'&&ss[1]!='/'){
                key/=visitFactor(ctx->factor()[i]);
            }else{
                p++;
                key^=visitFactor(ctx->factor()[i]);
            }
            ss.erase(0,p+ctx->factor()[i]->getText().length());
        }
        return key;
    }

    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        if(ctx->factor()!=NULL){
            string ss=ctx->getText();
            Element key=visitFactor(ctx->factor());
            if(ss[0]=='-')key=-key;
            return key;
        }
        else return visitAtom_expr(ctx->atom_expr());
    }

    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        if(ctx->trailer()!=NULL){
            if(ctx->atom()->NAME()){
                //visitAtom(ctx->atom());
                func=nf[ctx->atom()->NAME()->getText()];
                if(func==1){
                    visitTrailer(ctx->trailer());
                }else{
                    cnt++;
                    for(int i=0;i<vf[func].size();i++){
                        auto p=vf[func][i];
                        if(ff[func][p].fl!=0)g[cnt][p]=ff[func][p];
                    }
                    visitTrailer(ctx->trailer());
                    cnt--;
                }
                func=0;
                return Element("",0);
            }
        }else{
            Element key=visitAtom(ctx->atom());
            return key;
        }
    }

    virtual antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        if(ctx->arglist()!=NULL)return visitArglist(ctx->arglist());
    }

    virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        //cout<<ctx->getText()<<endl;
        //if(ctx->STRING().size())cout<<ctx->STRING()[0]->getText()<<endl;
        if(ctx->NAME()!=NULL){
            //cout<<ctx->NAME()->getText();
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
            tmp.push_back(visitTest(c));
        }
        return tmp;
    }

    virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        Element tmp=visitArgument(ctx->argument()[0]);
        if(func!=1&&ctx->argument()[0]->NAME()==NULL)g[cnt][vf[func][0]]=tmp;
        for(int i=1;i<ctx->argument().size();i++){
            if(func==1)putchar(' ');
            tmp=visitArgument(ctx->argument()[i]);
            if(func!=1&&ctx->argument()[0]->NAME()==NULL)g[cnt][vf[func][i]]=tmp;
        }
        if(func==1)puts("");
        return tmp;
    }

    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        Element key("",0);
        if(ctx->NAME()!=NULL) {
            const auto &c=ctx->NAME();
            g[cnt][c->getText()]=visitTest(ctx->test());
            key=g[cnt][c->getText()];
        }else{
            key=visitTest(ctx->test());
        }
        if(func==1)key.print();
        return key;
    }
};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H
