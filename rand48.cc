#include <nan.h>

#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <algorithm>

#define NRAND48_MAX 2147483648u

using namespace v8;

class Rand48 : public node::ObjectWrap
{
	public:
		static void Init(v8::Local<v8::Object> exports);
		long int rand(long int min, long int max);

	private:
		explicit Rand48(unsigned short int psubi[3]);
		~Rand48();

		static NAN_METHOD(New);
		static NAN_METHOD(Rand);
		static v8::Persistent<v8::Function> constructor;

		struct drand48_data 	drand_buffer;
		unsigned short int 		xsubi[3];
};

Persistent<Function> Rand48::constructor;

Rand48::Rand48(unsigned short int psubi[3])
{
	xsubi[0]=psubi[0];
	xsubi[1]=psubi[1];
	xsubi[2]=psubi[2];

	seed48_r(xsubi, &drand_buffer);
}

Rand48::~Rand48()
{
}

long int Rand48::rand(long int min, long int max)
{
	long int r;
	nrand48_r(xsubi, &drand_buffer, &r);

	return (long int)(min + r / (NRAND48_MAX / ((max+1)-min) + 1));
}

void Rand48::Init(Local<Object> exports)
{
	// Prepare constructor template
	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("Rand48").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetPrototypeMethod(tpl, "rand", Rand);

	//Nan::AssignPersistent(constructor, tpl->GetFunction());
	exports->Set(Nan::New("Rand48").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(Rand48::New)
{
	unsigned short int psubi[3];

	if(info.IsConstructCall())
	{
		if(info[0]->IsUndefined())
		{
			struct timeval now;
			gettimeofday(&now, 0);
			psubi[0] = (unsigned short int)now.tv_usec;
			psubi[1] = (unsigned short int)(now.tv_usec>>16);
			psubi[2] = getpid();
		}
		else
		{
			psubi[0] = info[0]->NumberValue();
			psubi[1] = info[1]->IsUndefined() ? 0 : info[1]->NumberValue();
			psubi[2] = info[2]->IsUndefined() ? 0 : info[2]->NumberValue();
		}

		Rand48 * obj = new Rand48(psubi);
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	}
}

NAN_METHOD(Rand48::Rand)
{
	if(info.Length()!=2)
	{
		Nan::ThrowTypeError("Wrong number of arguments");
    	info.GetReturnValue().Set(Nan::Undefined());
    	return;
	}

	if(!info[0]->IsInt32() || !info[1]->IsInt32())
	{
		char buf[1024];
		sprintf(buf, "rand(min, max) arguments must be between 0 and %u", NRAND48_MAX);
		Nan::ThrowTypeError(buf);
    	info.GetReturnValue().Set(Nan::Undefined());
    	return;
	}

	if(info[1]->Int32Value()<info[0]->Int32Value())
	{
		Nan::ThrowTypeError("rand(min, max) min MUST be greather than or equal to max");
    	info.GetReturnValue().Set(Nan::Undefined());
    	return;
	}

	Rand48 * r = Rand48::Unwrap<Rand48>(info.Holder());
	info.GetReturnValue().Set(Nan::New<v8::Int32>((int32_t)r->rand(info[0]->Int32Value(), info[1]->Int32Value())));
}

void Init(Local<Object> exports)
{
	Rand48::Init(exports);
}

NODE_MODULE(rand48, Init)
