/*
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */

#include "rc.h"
#include "exec.h"
#include "io.h"
#include "fns.h"
tree *treenodes;
/*
 * create and clear a new tree node, and add it
 * to the node list.
 */

tree*
newtree(void)
{
	tree *t = new(tree);
	t->iskw = 0;
	t->str = 0;
	t->child[0] = t->child[1] = t->child[2] = 0;
	t->next = treenodes;
	treenodes = t;
	return t;
}

void
freenodes(void)
{
	tree *t, *u;
	for(t = treenodes;t;t = u){
		u = t->next;
		if(t->str)
			efree(t->str);
		efree((char *)t);
	}
	treenodes = 0;
}

tree*
tree1(int type, tree *c0)
{
	return tree3(type, c0, (tree *)0, (tree *)0);
}

tree*
tree2(int type, tree *c0, tree *c1)
{
	return tree3(type, c0, c1, (tree *)0);
}

tree*
tree3(int type, tree *c0, tree *c1, tree *c2)
{
	tree *t;
	if(type==';'){
		if(c0==0)
			return c1;
		if(c1==0)
			return c0;
	}
	t = newtree();
	t->type = type;
	t->child[0] = c0;
	t->child[1] = c1;
	t->child[2] = c2;
	return t;
}

tree*
treeeq(int type, tree *c0, tree *c1)
{
	char *old;
	old = c0->str;
	c0->str = smprint("%s=", old);
	c0->quoted = 1;
	free(old);
	return tree2('^', c0, c1);
}

tree*
mung1(tree *t, tree *c0)
{
	t->child[0] = c0;
	return t;
}

tree*
mung2(tree *t, tree *c0, tree *c1)
{
	t->child[0] = c0;
	t->child[1] = c1;
	return t;
}

tree*
mung3(tree *t, tree *c0, tree *c1, tree *c2)
{
	t->child[0] = c0;
	t->child[1] = c1;
	t->child[2] = c2;
	return t;
}

tree*
epimung(tree *comp, tree *epi)
{
	tree *p;
	if(epi==0)
		return comp;
	for(p = epi;p->child[1];p = p->child[1]);
	p->child[1] = comp;
	return epi;
}
/*
 * Add a SIMPLE node at the root of t and percolate all the redirections
 * up to the root.
 */

tree*
simplemung(tree *t)
{
	tree *u;
	struct io *s;

	t = tree1(SIMPLE, t);
	s = openstr();
	pfmt(s, "%t", t);
	t->str = strdup((char *)s->strp);
	closeio(s);
	for(u = t->child[0];u->type==ARGLIST;u = u->child[0]){
		if(u->child[1]->type==DUP
		|| u->child[1]->type==REDIR){
			u->child[1]->child[1] = t;
			t = u->child[1];
			u->child[1] = 0;
		}
	}
	return t;
}

tree*
token(char *str, int type)
{
	tree *t = newtree();

	t->type = type;
	t->str = strdup(str);
	return t;
}

void
freetree(tree *p)
{
	if(p==0)
		return;	
	freetree(p->child[0]);
	freetree(p->child[1]);
	freetree(p->child[2]);
	if(p->str)
		efree(p->str);
	efree((char *)p);
}
