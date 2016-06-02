/*-------------------------------------------------------------*/
/*  CLB - Cudne LB - Stencil Version                           */
/*     CUDA based Adjoint Lattice Boltzmann Solver             */
/*     Author: Lukasz Laniewski-Wollk                          */
/*     Developed at: Warsaw University of Technology - 2012    */
/*-------------------------------------------------------------*/

/*
Model for solving the wave equation as a system of first order DE's
                  u'' = c(u_xx + u_yy)
*/

CudaDeviceFunction float2 Color() {
// used for graphics - can usually ignore function
	float2 ret;
	ret.x = 0 ;
	ret.y = 1 ;
	return ret;
}
CudaDeviceFunction void Init() {
// Initialise the velocity at each node
	u = Value;
	v = 0;
}
CudaDeviceFunction void Run() {
// what function would you like to do at each node
  real_t lap_u = u(-1,0) + u(1,0) + u(0,-1) + u(0,1) - 4*u(0,0);
  real_t lap_v = v(-1,0) + v(1,0) + v(0,-1) + v(0,1) - 4*v(0,0);
  real_t a; 
  <?R
	a = Speed * Speed * lap_u + Viscosity * lap_v
  ?>
  v = v(0,0) + a;
  u = u(0,0) + v;
  if ((NodeType & NODE_BOUNDARY) == NODE_Dirichlet) {
    u = Value;
    v = 0;
}
}


CudaDeviceFunction real_t getU() {
// return the value at the current node for export
	return u(0,0);
}
