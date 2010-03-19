/**
 * FaZend.com, Fully Automated Zend Framework
 * RQDQL.com, Requirements Definition and Query Language
 *
 * Redistribution and use in source and binary forms, with or 
 * without modification, are PROHIBITED without prior written 
 * permission from the author. This product may NOT be used 
 * anywhere and on any computer except the server platform of 
 * FaZend.com. located at www.fazend.com. If you received this 
 * code occasionally and without intent to use it, please report 
 * this incident to the author by email: team@rqdql.com
 *
 * @author Yegor Bugayenko <egor@tpc2.com>
 * @copyright Copyright (c) rqdql.com, 2010
 * @version $Id$
 */

// we should track locations of errors, in YYLTYPE, see lyyerror()
%locations

%union {
    char* name;
};

// Here we should say that the type of non-terminal
// terms are mapped to %union.name, and are strings because of that
%type <name> actions
%type <name> action
%type <name> verbs
%type <name> verb
%type <name> predicates
%type <name> predicate
%type <name> words
%type <name> modifier
%type <name> subjects
%type <name> subject
%type <name> object
%type <name> attribute

// Declaration of all known tokens
%token <name> INFORMAL
%token <name> ENTITY
%token <name> FUR
%token <name> ACTOR
%token <name> UC
%token <name> WORD
%token <name> ACRONYM 
%token <name> TBD
%token <name> ATTRIBS

%token COLON SEMICOLON DOT COMMA
%token SEE
%token AND OR
%token CAN
%token IF
%token OF
%token USING
%token THIS
%token PLURAL_MANY PLURAL_SOME PLURAL_ANY
%token OPEN_BRACE CLOSE_BRACE
%token IS_A INCLUDES PRODUCES

%nonassoc COMMA
%nonassoc AND
%nonassoc SEMICOLON
%right OF
%right COLON
%right INCLUDES
%right PRODUCES

%{
    #include "rqdql.h"
%}

%%

SRS:
    /* it can be empty */ |
    SRS DottedStatement;

DottedStatement:
    Statement DOT;

Statement:
    FurStatement { rqdql::log(rqdql::info, "FUR statement processed"); } |
    EntityStatement { rqdql::log(rqdql::info, "entity statement processed"); } | 
    SeeStatement 
    ;

FurStatement:
    lfur COLON actions |
    lfur COLON TBD |
    lfur error { lyyerror(@2, "colon expected after '%s'", $1); } |
    lfur COLON error { lyyerror(@3, "actions expected after '%s:'", $1); } 
    ;
    
/* left FUR */
lfur:
    FUR |
    FUR ATTRIBS
    ;
    
actions:
    action |
    actions SEMICOLON action
    ;
    
action:
    ACTOR CAN verbs subjects |
    ACTOR error { lyyerror(@2, "'can' missed after '%s'", $1); } |
    ACTOR CAN error { lyyerror(@3, "list of verbs not found after '%s can'", $1); } |
    ACTOR CAN verbs error { lyyerror(@4, "list of subjects missed after '%s can %s'", $1, $3); }
    ;
    
verbs:
    verb |
    verbs separator verb { $$ = rqdql::sprintf("%s, %s", $1, $3); }
    ;
    
verb:
    ACRONYM |
    WORD |
    WORD modifier
    ;
    
subjects:
    subject |
    subjects separator subject
    ;
    
separator:
    COMMA |
    AND |
    COMMA AND
    ;
    
subject:
    object |
    object modifier { $$ = rqdql::sprintf("%s %s", $1, $2); }
    ;
    
object:
    THIS { $$ = rqdql::sprintf("this"); } |
    ACTOR plural { $$ = rqdql::sprintf("%s", $1); } |
    ENTITY plural { $$ = rqdql::sprintf("%s", $1); } |
    attribute OF object { $$ = rqdql::sprintf("%s of %s", $1, $3); }
    ;
    
attribute:
    words
    ;
    
words:
    WORD |
    words WORD { $$ = rqdql::sprintf("%s %s", $1, $2); }
    ;
    
plural:
    /* singular */ | PLURAL_MANY | PLURAL_SOME | PLURAL_ANY;

modifier:
    OPEN_BRACE predicates CLOSE_BRACE { $$ = rqdql::sprintf("%s", $2); }
    ;
    
predicates:
    predicate |
    predicates SEMICOLON predicate
    ;
    
predicate:
    lambda subjects { $$ = rqdql::sprintf("L %s", $2); } |
    lambda subjects verbs { $$ = rqdql::sprintf("L %s %s", $2, $3); }
    ;
    
lambda:
    IF |
    USING
    ;
    
EntityStatement:
    lobject COLON INFORMAL |
    lobject IS_A object |
    lobject INCLUDES parts |
    lobject PRODUCES parts
    ;
    
lobject:
    ENTITY |
    ACTOR
    ;

parts:
    part |
    parts SEMICOLON part
    ;

part:
    attribute |
    attribute COLON INFORMAL |
    attribute INCLUDES parts 
    ;
    
/* See: R4.4, ActorUser, ... */
SeeStatement:
    SEE COLON entities
    ;
    
entities:
    entity |
    entities separator entity
    ;
    
entity: 
    FUR |
    UC |
    subject 
    ;

%%

// see global.cpp