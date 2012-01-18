#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <algorithm>

#define NRAND48_MAX 2147483648u

using namespace v8;
using namespace node;

class Rand48 : public ObjectWrap
{
	private:
		struct drand48_data 	drand_buffer;
		unsigned short int 		xsubi[3];

	public:
		static Persistent<FunctionTemplate> s_ct;
		static void Initialize(v8::Handle<v8::Object> target)
		{
			HandleScope scope;

			Local<FunctionTemplate> t = FunctionTemplate::New(New);

			s_ct = Persistent<FunctionTemplate>::New(t);
			s_ct->InstanceTemplate()->SetInternalFieldCount(1);
			s_ct->SetClassName(String::NewSymbol("Rand48"));

			NODE_SET_PROTOTYPE_METHOD(s_ct, "rand", rand);

			target->Set(String::NewSymbol("Rand48"), t->GetFunction());
		}

		long int rand(long int min, long int max)
		{
			long int r;
			nrand48_r(xsubi, &drand_buffer, &r);

			return (long int)(min + r / (NRAND48_MAX / ((max+1)-min) + 1));
		}

		Rand48(unsigned short int psubi[3]) : ObjectWrap ()
		{
			xsubi[0]=psubi[0];
			xsubi[1]=psubi[1];
			xsubi[2]=psubi[2];
			seed48_r(xsubi, &drand_buffer);
		}

		~Rand48()
		{

		}

	protected:
		static Handle<Value> New(const Arguments& args)
		{
			HandleScope scope;

			unsigned short int xsubi[3];
			int argsLength = args.Length();
			if(argsLength>0)
			{
				int i=0;
				for(i=0;i<std::min(argsLength, 3);i++)
				{
					if(!args[i]->IsInt32())
					{
						char buf[1024];
						sprintf(buf, "Constructor seed arguments must be numbers between 0 and %d", USHRT_MAX);
						return ThrowException(Exception::TypeError(String::New(buf)));
					}

					int val = args[i]->Int32Value();
					if(val<0 || val>USHRT_MAX)
					{
						char buf[1024];
						sprintf(buf, "Constructor seed arguments must be numbers between 0 and %d", USHRT_MAX);
						return ThrowException(Exception::TypeError(String::New(buf)));
					}

					xsubi[i] = (unsigned short int)val;
				}

				for(;i<3;i++)
				{
					xsubi[i] = (unsigned short int)(xsubi[i-1]*7);
				}
			}
			else
			{
				struct timeval now;
				gettimeofday(&now, 0);
				xsubi[0] = (unsigned short int)now.tv_usec;
				xsubi[1] = (unsigned short int)(now.tv_usec>>16);
				xsubi[2] = getpid();
			}

			Rand48* rand48 = new Rand48(xsubi);
			rand48->Wrap(args.This());
			return args.This();
		}

		static Handle<Value> rand(const Arguments& args)
		{
			HandleScope scope;

			if(args.Length()<2)
				return ThrowException(Exception::Error(String::New("Usage: rand(min, max)")));

			if(!args[0]->IsInt32() || !args[1]->IsInt32())
			{
				char buf[1024];
				sprintf(buf, "rand(min, max) arguments must be betwee 0 and %d", NRAND48_MAX);
				return ThrowException(Exception::TypeError(String::New(buf)));
			}

			Rand48* rand48 = ObjectWrap::Unwrap<Rand48>(args.This());
			return scope.Close(Int32::New(rand48->rand(args[0]->Int32Value(), args[1]->Int32Value())));
		}
};

Persistent<FunctionTemplate> Rand48::s_ct;

extern "C" void init(Handle<Object> target)
{
	HandleScope scope;
	Rand48::Initialize(target);
}

//https://www.cloudkick.com/blog/2010/aug/23/writing-nodejs-native-extensions/

/*
 void seed_rand(int thread_n, struct drand48_data *buffer)
 {
 struct timeval tv;

 gettimeofday(&tv, NULL);
 srand48_r(tv.tv_sec * thread_n + tv.tv_usec, buffer);
 }

 void *threadFunc(void *arg)
 {
 struct thread_info *thread_info = arg;
 struct drand48_data drand_buffer;

 int n = 0;
 const int rays = thread_info->rays;
 int hits_in = 0;
 double x;
 double y;
 double r;

 seed_rand(thread_info->thread_n, &drand_buffer);

 for (n = 0; n < rays; n++)
 {
 drand48_r(&drand_buffer, &x);
 drand48_r(&drand_buffer, &y);
 r = x * x + y * y;
 if (r < 1.0){
 hits_in++;
 }
 }

 thread_info->hits_in = hits_in;
 return NULL;
 }*/
