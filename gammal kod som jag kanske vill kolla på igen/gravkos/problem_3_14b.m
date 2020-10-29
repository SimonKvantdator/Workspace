clc; clear;
syms u v x y xi_u(u,v,x,y) xi_v(u,v,x,y) xi_x(u,v,x,y) xi_y(u,v,x,y) a(u) b(u)

eqns = [diff(xi_u, u) == 0;
        diff(xi_u, v) + diff(xi_v, u) == 0;
        diff(xi_v, v) == 0;
        diff(xi_u, x) + diff(xi_x, u) == 0;
        -2 * a * diff(a, u) * xi_x + diff(xi_v, x) + diff(xi_x, v) == 0;
        -(diff(a, u) / a) * xi_u + diff(xi_x, x) == 0;
        diff(xi_u, y) + diff(xi_y, u) == 0;
        -2 * b * diff(b, u) + diff(xi_v, y) + diff(xi_y, v) == 0;
        diff(xi_x, y) + diff(xi_y, x) == 0;
        -(diff(b, u) / b) xi_u + diff(xi_y, y) == 0]


S = dsolve(eqns)




