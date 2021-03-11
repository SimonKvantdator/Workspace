
/*Given pointers to a_re, a_im, b_re, b_im, c_re, and c_im, returns the product of the complex numbers b and c in a*/
void mul_cpx(double* a_re, double* a_im, double* b_re, double* b_im, double* c_re, double* c_im) {
	a_re[0] = b_re[0] * c_re[0] - b_im[0] * c_im[0];
	a_im[0] = b_re[0] * c_im[0] + b_im[0] * c_re[0];
}
