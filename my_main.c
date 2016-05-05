/*========== my_main.c ==========

  This is the only file you need to modify in order
  to get a working mdl project (for now).

  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].

  Your job is to go through each entry in op and perform
  the required action from the list below:

  push: push a new origin matrix onto the origin stack
  pop: remove the top matrix on the origin stack

  move/scale/rotate: create a transformation matrix 
                     based on the provided values, then 
		     multiply the current top of the
		     origins stack by it.

  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a 
		    temporary matrix, multiply it by the
		    current top of the origins stack, then
		    call draw_polygons.

  line: create a line based on the provided values. Store 
        that in a temporary matrix, multiply it by the
	current top of the origins stack, then call draw_lines.

  save: call save_extension with the provided filename

  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "misc_headers.h"
#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

void my_main( int polygons ) {

  int i;
  double step;
  double xval, yval, zval;
  struct matrix *transform;
  struct matrix *tmp;
  struct stack *s;
  screen t;
  color g;
  
  s = new_stack();
  tmp = new_matrix(4, 1000);
  clear_screen( t );
  
  for (i=0;i<lastop;i++) {  
    switch (op[i].opcode) {
      switch (op[i].opcode) {
      case PUSH:
        push(s);
        break;
        
      case POP:
        pop(s);
        break;
	
      case MOVE:
        tmp = make_translate(op[i].op.move.d[0], op[i].op.move.d[1], op[i].op.move.d[2]);
        matrix_mult(s->data[s->top], tmp);
        copy_matrix(tmp, s->data[s->top]);
        break;
	
      case SCALE:
        tmp = make_scale(op[i].op.scale.d[0], op[i].op.scale.d[1], op[i].op.scale.d[2]);
        matrix_mult(s->data[s->top], tmp);
        copy_matrix(tmp, s->data[s->top]);
        break;   
        
      case ROTATE:
	double theta;
        theta = op[lastop].op.rotate.degrees * (M_PI / 180);
        if (op[i].op.rotate.axis == 0) {
          tmp = make_rotX(theta);
        }
	else if (op[i].op.rotate.axis == 1) {
          tmp = make_rotY(theta);
        }
	else if (op[i].op.rotate.axis == 2) {
          tmp = make_rotZ(theta);
        }
	
        matrix_mult(s->data[s->top], tmp);
        copy_matrix(tmp, s->data[s->top]);
	
        break;
      
      case BOX:
        add_box(tmp, op[i].op.box.d0[0], op[i].op.box.d0[1], op[i].op.box.d0[2], op[i].op.box.d1[0], op[i].op.box.d1[1], op[i].op.box.d1[2]);
	
        matrix_mult(s->data[s->top], tmp);
	
        draw_polygons(tmp, t, g);
	
        break;
        
      case SPHERE:
        add_sphere(tmp, op[i].op.sphere.d[0], op[i].op.sphere.d[1], op[i].op.sphere.d[2], op[i].op.sphere.r, 10);
        matrix_mult(s->data[s->top], tmp);
	
        draw_polygons(tmp, t, g);
	
        break;
        
      case TORUS:
        add_torus(tmp, op[i].op.torus.d[0], op[i].op.torus.d[1], op[i].op.torus.d[2], op[i].op.torus.r0, op[i].op.torus.r1, 10);
        matrix_mult(s->data[s->top], tmp);
	
        draw_polygons(tmp, t, g);
        break;
        
      case LINE:
        add_edge(tmp, op[i].op.line.p0[0], op[i].op.line.p0[1], op[i].op.line.p0[2], op[i].op.line.p1[0], op[i].op.line.p1[1], op[i].op.line.p1[2]);
	
        matrix_mult(s->data[s->top], tmp);
	
        draw_lines(tmp, t, g);
        break;
        
      case SAVE:
        save_extension(t, op[i].op.save.p -> name);
	
        break;
    
      case DISPLAY:
        display(t);;
	
        break;
      case COMMENT:
	break;
        
      default:
        break;
    }
  }
}
