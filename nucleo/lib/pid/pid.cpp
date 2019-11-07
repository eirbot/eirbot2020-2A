/*
 * TODO
 * Documentation
 * Make position and angle pid to control motor's speed pid
 */

#include "pid.hpp"


CArray::CArray(int _length)
{
	length = _length;
	array = new float[length];
	reset();
}

CArray::~CArray()
{
	delete [] array;
}

void CArray::reset()
{
	index = 0;
	for (int i = 0; i < length; i++) {
		array[i] = 0.0f;
	}
}

void CArray::add(float val)
{
	index = ((index-1) % length + length) % length;
	array[index] = val;
}

float CArray::operator[](int _index)
{
	return array[(index+_index) % length];
}


Pid::Pid(float* _coef_err, int _len_err, float* _coef_co, int _len_co):
	err_ca(_len_err),
	co_ca(_len_co)
{
	len_err = _len_err;
	len_co = _len_co;
	coef_err = _coef_err;
	coef_co = _coef_co;
}

Pid::~Pid()
{

}

void Pid::reset()
{
	err_ca.reset();
	co_ca.reset();
}

float Pid::getPid()
{
	float sum = 0.0f;
	for (int i = 0; i < len_err; i++) {
		sum += coef_err[i] * err_ca[i];
	}
	for (int i = 0; i < len_co; i++) {
		sum -= coef_co[i+1] * co_ca[i];
	}
	return sum;
}

float Pid::getPid(float _err, float _co)
{
	err_ca.add(_err);
	co_ca.add(_co);
	return getPid();
}
