/**

 * RQDQL.com, Requirements Definition and Query Language
 *
 * Redistribution and use in source and binary forms, with or 
 * without modification, are PROHIBITED without prior written 
 * permission from the author. This product may NOT be used 
 * anywhere and on any computer except the server platform of 
 * rqdql.com. located at www.rqdql.com. If you received this 
 * code occasionally and without intent to use it, please report 
 * this incident to the author by email: team@rqdql.com
 *
 * @author Yegor Bugayenko <egor@tpc2.com>
 * @copyright Copyright (c) rqdql.com, 2010
 * @version $Id$
 */

// we should track locations of errors, in YYLTYPE, see lyyerror()
%locations

%{
    #include "rqdql.h"
    #include "Proxy.h"
    #include "Solm.h"
    #include "Solm/Formula/Predicate.h"
    #include "Proxy/Type.h"
    #include "brokers/De.h"
    #include "brokers/FlowHolder.h"
    #include "brokers/SigElement.h"
    #include "brokers/AltPair.h"
    #include "brokers/SignatureHolder.h"
	using boost::format;    
%}

// Here we should say that the type of non-terminal
// terms are mapped to %union.name, and are strings because of that
%type <name> words
%type <name> slotName
%type <name> objectName
%type <name> informal
%type <name> verb
%type <ptr> theClass
%type <ptr> classPath
%type <ptr> slots
%type <ptr> slot
%type <ptr> predicate
%type <ptr> invariant
%type <ptr> classDefinition
%type <ptr> useCaseDefinition
%type <ptr> invariantDeclaration
%type <ptr> slotsDeclaration
%type <ptr> useCaseDeclaration
%type <ptr> useCaseAlternativeDeclaration
%type <ptr> ucStarter
%type <ptr> signature
%type <ptr> sigElements
%type <ptr> sigElement
%type <ptr> de
%type <ptr> deType
%type <ptr> flows
%type <ptr> flow
%type <ptr> altId
%type <ptr> altIdPair


// Declaration of all known tokens
%token <name> QUOTED
%token <name> CAMEL
%token <name> QOS
%token <name> UC
%token <name> WORD
%token <num> NUMBER
%token <name> LETTER
%token <name> PREPOSITION

%token COLON SEMICOLON DOT COMMA STAR
%token AND OR
%token WHERE
%token SUD
%token SOMEBODY
%token SOMETHING
%token THE
%token OF
%token IF
%token SELF
%token PLURAL_MANY PLURAL_SOME PLURAL_ANY
%token OPEN_BRACE CLOSE_BRACE
%token IS_A
%token INCLUDES

%nonassoc COMMA
%nonassoc AND
%nonassoc SEMICOLON
%nonassoc DOT
%nonassoc PREPOSITION
%left WORD
%left NUMBER
%right OF
%right COLON
%right INCLUDES

%%

srs:
    /* it can be empty */ 
    |
    srs statement 
    ;

statement:
    classDefinition  
    | 
    useCaseDefinition  
    | 
    qosDefinition 
    |
    informal DOT
    |
    error DOT
        {
            lyyerror(@1, "Statement ignored"); 
        }
    ;

classDefinition:
    invariantDeclaration |
    slotsDeclaration
    ;
    
useCaseDefinition:
    useCaseDeclaration |
    useCaseAlternativeDeclaration
    ;

qosDefinition:
    QOS COLON informal DOT
    ;
    
/** 
 * Invariants... 
 */
invariantDeclaration: /* proxy::Type* */
    classPath IS_A invariant DOT 
        { 
            if (!$1) {
                lyyerror(@3, "You can't place HIMSELF to the left part of declaration");
            } else {
                proxy::Type* classPath = static_cast<proxy::Type*>($1);
                solm::Formula* invariant = static_cast<solm::Formula*>($3);
                classPath->predicate() += *invariant;
                delete invariant;
                $$ = classPath;
                protocol(@1, classPath);
            }
        } 
    ;
    
invariant: /* solm::Predicate* */
    predicate 
    |
    predicate informal
    ;
    
predicate: /* solm::Predicate* */
    informal
        {
            solm::Predicate* predicate = new solm::Predicate("info");
            std::string* informal = static_cast<std::string*>($1);
            p->arg("'" + *informal);
            delete informal;
            $$ = predicate;
            protocol(@1, predicate);
        } 
    |
    words
        {
            solm::Predicate* predicate = new solm::Predicate("info");
            std::string* words = static_cast<std::string*>($1);
            p->arg("'words: " + *words);
            delete words;
            $$ = predicate;
            protocol(@1, predicate);
        }
    |
    WORD
        {
            solm::Predicate* predicate = new solm::Predicate("info");
            std::string* WORD = static_cast<std::string*>($1);
            p->arg("'word: " + *WORD);
            delete WORD;
            $$ = predicate;
            protocol(@1, predicate);
        }
    |
    theClass plurality
        {
            if (!$1) {
                lyyerror(@1, "Class can't be an instance of himself");
            } else {
                proxy::Type* theClass = static_cast<proxy::Type*>($1);
                solm::Predicate* predicate = new solm::Predicate(theClass); // by name of type
                predicate->arg("x");
                $$ = predicate;
                protocol(@1, predicate);
            }
        }
    ;

/**
 * Slots... 
 */
slotsDeclaration: /* proxy::Type* */
    classPath INCLUDES COLON slots DOT 
        {
            proxy::Type* classPath = static_cast<proxy::Type*>($1);
            proxy::Type::Slots* slots = static_cast<proxy::Type::Slots*>($4);
            *classPath += *slots; // add all slots to the type
            delete slots;
            $$ = classPath;
            protocol(@1, classPath);
        }
    ;
    
classPath: /* proxy::Type* */
    theClass 
        {
            if (!$1) {
                lyyerror(@1, "HIMSELF is forbidden here, in class path");
            } else {
                proxy::Type* theClass = static_cast<proxy::Type*>($1);
                $$ = theClass;
                protocol(@1, theClass);
            }
        }
    |
    slotName OF classPath
        {
            std::string* slotName = static_cast<std::string*>($1);
            proxy::Type* classPath = static_cast<proxy::Type*>($3);
            proxy::Type* classPathLhs = &(classPath->slot(*slotName).type());
            delete slotName;
            $$ = classPathLhs;
            protocol(@1, classPathLhs);
        }
    ;
    
slots: /* std::vector<proxy::Slot*>* */
    slot
        {
            proxy::Type::Slots* slots = new proxy::Type::Slots();
            proxy::Slot* slot = static_cast<proxy::Slot*>($1);
            slots->push_back(*slot);
            delete slot;
            $$ = slots;
            protocol(@1, slots);
        }
    |
    slots separator slot 
        {
            proxy::Type::Slots* slotsLhs = new proxy::Type::Slots();
            proxy::Type::Slots* slots = static_cast<proxy::Type::Slots*>($1);
            proxy::Slot* slot = static_cast<proxy::Slot*>($1);
            for (Type::Slots::const_iterator i = slots->begin(); i != slots->end(); ++i) {
                slotsLhs->push_back(*i);
            }
            slotsLhs->push_back(slot);
            $$ = slotsLhs;
            protocol(@1, slotsLhs);
        } 
    |
    slots separator error
        {
            lyyerror(@3, "Slot ignored"); 
        }
    ;
    
slot: /* proxy::Slot* */
    slotName
        {
            std::string* slotName = static_cast<std::string*>($1);
            $$ = new proxy::Slot(*slotName);
            delete slotName;
            protocol(@1, $$);
        }
    |
    slotName COLON invariant
        { 
            std::string* slotName = static_cast<std::string*>($1);
            solm::Predicate* invariant = static_cast<std::string*>($3);
            proxy::Slot* slot = new proxy::Slot(*slotName);
            slot->predicate() += *invariant;
            delete slotName;
            delete invariant;
            protocol(@1, slot);
        }
    ;
    
/**
 * Use cases... 
 */
useCaseDeclaration: /* NULL */
    ucStarter flows 
        {
            proxy::UseCase* ucStarter = static_cast<proxy::UseCase*>($1);
            proxy::Flows* flows = static_cast<proxy::Flows*>($2);
            *uc = *flows; // set full collection of flows
            delete flows;
        } 
    |
    ucStarter informal DOT
        {
            proxy::UseCase* ucStarter = static_cast<proxy::UseCase*>($1);
            std::string* informal = static_cast<std::string*>($2);
            // @todo To make explicit injection into SOLM of just this formula
            // in a form of declaration
            delete informal;
        }
    ;
     
ucStarter: /* proxy::UseCase */
    UC WHERE signature COLON 
        {
            std::string* UC = static_cast<std::string*>($1);
            brokers::SignatureHolder* signature = static_cast<brokers::SignatureHolder*>($3);
            if (!signature->hasSignature()) {
                lyyerror(@1, "Use Case signature can't be completely informal");
            } else {
                proxy::UseCase* ucStarter = rqdql::get<proxy::Proxy>().entity(*UC);
                sh->getSignature();
                $$ = uc;
                protocol(@1, $$);
            }
        }
    ;
    
/**
 * V
 * O V O V ...
 * O V O V ... O
 */
signature: /* brokers::SignatureHolder* */
    informal 
        {
            brokers::SignatureHolder* s = new brokers::SignatureHolder();
            s->setText(*$1);
            $$ = s;
            protocol(@1, $$);
        }
    |
    sigElements 
        {
            brokers::SignatureHolder* s = new brokers::SignatureHolder();
            s->setSignature(static_cast<brokers::SigElements*>($1));
            $$ = s;
            protocol(@1, $$);
        }
    |
    sigElements informal 
        {
            brokers::SignatureHolder* s = new brokers::SignatureHolder();
            s->setSignature(static_cast<brokers::SigElements*>($1));
            s->setText(s->getText() + " " + *$2);
            $$ = s;
            protocol(@1, $$);
        }
    ;
    
sigElements: /* brokers::SigElements* */
    sigElement
        {
            brokers::SigElements* v = new brokers::SigElements();
            v->push_back(static_cast<brokers::SigElement*>($1));
            $$ = v;
            protocol(@1, $$);
        }
    |
    sigElements sigElement
        {
            brokers::SigElements* v = new brokers::SigElements();
            brokers::SigElements* e = static_cast<brokers::SigElements*>($1);
            for (brokers::SigElements::const_iterator i = e->begin(); i != e->end(); ++i) {
                v->push_back(*i);
            }
            v->push_back(static_cast<brokers::SigElement*>($2));
            $$ = v;
            protocol(@1, $$);
        }
    ;
    
sigElement: /* brokers::SigElement* */
    de
        {
            brokers::SigElement* se = new brokers::SigElement();
            se->setDe(static_cast<brokers::De*>($1));
            $$ = se;
            protocol(@1, $$);
        }
    |
    verb 
        {
            brokers::SigElement* se = new brokers::SigElement();
            se->setVerb(*$1);
            $$ = se;
            protocol(@1, $$);
        }
    |
    informal de 
        {
            brokers::SigElement* se = new brokers::SigElement();
            se->setInformal(*$1);
            se->setDe(static_cast<brokers::De*>($2));
            $$ = se;
            protocol(@1, $$);
        }
    |
    informal verb
        {
            brokers::SigElement* se = new brokers::SigElement();
            se->setInformal(*$1);
            se->setVerb(*$2);
            $$ = se;
            protocol(@1, $$);
        }
    ;

de: /* brokers::De* */
    objectName 
        { 
            brokers::De* de = new brokers::De();
            de->setName(*$1);
            $$ = de;
            protocol(@1, $$);
        }
    |
    deType 
        {
            brokers::De* de = new brokers::De();
            de->setExplanation(static_cast<Signature::Explanation*>($1));
            $$ = de;
            protocol(@1, $$);
        }
    |
    deType OPEN_BRACE objectName CLOSE_BRACE
        {
            brokers::De* de = new brokers::De();
            de->setExplanation(static_cast<Signature::Explanation*>($1));
            de->setName(*$3);
            $$ = de;
            protocol(@1, $$);
        }
    ;
    
deType: /* Signature::Explanation* */
    theClass
        {
            Signature::Explanation* e;
            Type* t = static_cast<Type*>($1);
            // maybe it's SELF?
            if (!t) {
                e = new Signature::Explanation();
            } else {
                e = new Signature::Explanation(t);
            }
            $$ = e;
            protocol(@1, $$);
        }
    |
    slotName OF objectName 
        {
            Signature::Explanation* e = new Signature::Explanation(*$1, *$3);
            $$ = e;
            protocol(@1, $$);
        }
    ;
    
/**
 * w1 w2 TO - is it "(w1) (w2 TO)" or "(w1 w2 TO)"
 */    
verb:
    words PREPOSITION 
        {
            string* s = new string((format("%s %s") % *$1 % *$2).str());
            $$ = s;
        } 
    |
    WORD 
        {
            $$ = $1;
        }
    |
    PREPOSITION 
        {
            $$ = $1;
        }
    |
    WORD PREPOSITION 
        {
            string* s = new string((format("%s %s") % *$1 % *$2).str());
            $$ = s;
        } 
    ;
    
flows: /* Flows* */
    flow 
        {
            Flows* v = new Flows();
            brokers::FlowHolder* f = static_cast<brokers::FlowHolder*>($1);
            v->addFlow(f->getId(), f->getFlow());
            protocol(@1, $$ = v);
        }
    |
    flows flow
        {
            Flows* v = new Flows();
            typedef map<int, Flow*> FlowsList;
            FlowsList e = (static_cast<Flows*>($1))->getFlows();
            for (FlowsList::const_iterator i = e.begin(); i != e.end(); ++i) {
                v->addFlow((*i).first, (*i).second);
            }
            brokers::FlowHolder* f = static_cast<brokers::FlowHolder*>($2);
            v->addFlow(f->getId(), f->getFlow());
            protocol(@1, $$ = v);
        }
    ;
    
flow: /* brokers::FlowHolder* */
    NUMBER DOT signature DOT 
        {
            brokers::FlowHolder* f = new brokers::FlowHolder();
            brokers::SignatureHolder* s = static_cast<brokers::SignatureHolder*>($3);
            if (s->hasSignature()) {
                protocol(@3, s->getSignature());
                f->setFlow(new Flow(s->getText(), s->getSignature()));
            } else {
                f->setFlow(new Flow(s->getText()));
            }
            f->setId($1);
            $$ = f;
            protocol(@1, f->getFlow());
        }
    ;
    
/**
 * alternative flow of a use case
 */
useCaseAlternativeDeclaration:
    UC altId CLOSE_BRACE IF predicate COLON flows 
    {
        Flows* flows = rqdql::get<Proxy>().get<UseCase>(*$1);
        brokers::AltPairs* e = static_cast<brokers::AltPairs*>($2);
        Flow* dest = 0;
        for (brokers::AltPairs::const_iterator i = e->begin(); i != e->end(); ++i) {
            if (dest) {
                flows = dest->findAlternative((*(i-1))->getLetter());
            }
            if ((*i)->getNum() == -1) {
                lyyerror(@2, "STAR is not supported yet");
                dest = 0;
                break;
            } else {
                dest = flows->getFlow((*i)->getNum());
            }
        }
        if (dest) {
            // it's found, inject flows there!
            protocol(@1, dest);
            Flows* alt = dest->addAlternative(static_cast<solm::Formula*>($5));
            alt->setFlows(static_cast<Flows*>($7));
        }
    }
    ;
    
altId: /* brokers::AltPairs* */
    altIdPair
        {
            brokers::AltPairs* p = new brokers::AltPairs();
            p->push_back(static_cast<brokers::AltPair*>($1));
            $$ = p;
        }
    |
    altId altIdPair
        {
            brokers::AltPairs* p = new brokers::AltPairs();
            brokers::AltPairs* e = static_cast<brokers::AltPairs*>($1);
            for (brokers::AltPairs::const_iterator i = e->begin(); i != e->end(); ++i) {
                p->push_back(*i);
            }
            p->push_back(static_cast<brokers::AltPair*>($2));
            $$ = p;
        }
    ;
    
altIdPair: /* brokers::AltPair* */
    NUMBER LETTER 
        {
            $$ = new brokers::AltPair($1, $2->at(0));
        }
    |
    STAR LETTER
        {
            $$ = new brokers::AltPair(-1, $2->at(0));
        }
    ;
    
/**
 * global elementary things 
 */
words:
    WORD WORD
        {
            string* s = new string((format("%s %s") % *$1 % *$2).str());
            $$ = s;
        }
    |
    words WORD
        {
            string* s = new string((format("%s %s") % *$1 % *$2).str());
            $$ = s;
        }
    ;
    
informal:
    QUOTED
    ;
    
separator:
    SEMICOLON | 
    SEMICOLON AND | 
    COMMA | 
    COMMA AND |
    AND
    ;

theClass:
    CAMEL 
        {
            $$ = rqdql::get<Proxy>().get<Type>(*$1);
            protocol(@1, $$);
        }
    |
    SUD 
        {
            $$ = rqdql::get<Proxy>().get<Type>("SUD");
            protocol(@1, $$);
        }
    |
    SOMEBODY
        {
            $$ = rqdql::get<Proxy>().get<Type>("somebody");
            protocol(@1, $$);
        }
    |
    SOMETHING
        {
            $$ = rqdql::get<Proxy>().get<Type>("something");
            protocol(@1, $$);
        }
    |
    SELF
        {
            $$ = 0;
        }
    ;
    
plurality:
    /* empty */ |
    PLURAL_MANY |
    PLURAL_SOME |
    PLURAL_ANY
    ;
    
slotName:
    WORD |
    words
    ;
    
objectName:
    THE WORD { $$ = $2; }
    ;
    
%%

// see global.cpp