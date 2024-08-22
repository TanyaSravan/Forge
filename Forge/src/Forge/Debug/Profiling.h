#pragma once
#include <string>
#include <chrono>
#include <fstream>


namespace Forge {
	struct ProfileData {
		const char* name;
		long long start;
		float time;
	};

	struct InstrumentationSession {
		std::string name;
	};


	class Instrumentation {
	public:

		Instrumentation()
			:m_CurrentSession(nullptr), m_ProfileCount(0)
		{

		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };

		}

		void WriteHeader() {
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter() {
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void WriteProfile(const ProfileData& result) {
			if (m_ProfileCount++ > 0) {
				m_OutputStream << ",";
			}

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << result.time << ",";
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":\"0\",";
			m_OutputStream << "\"tid\":\"0\",";
			m_OutputStream << "\"ts\":" << result.start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void EndSession() {
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		static Instrumentation& Get() {
			static Instrumentation instance;
			return instance;
		}


	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	};

	class Timer {
	public:
		Timer(const char* name)
			: m_name(name), m_Stopped(false)
		{
			m_StartPoint = std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}

		~Timer() {
			if (!m_Stopped)
				Stop();
		}

		void Stop() {
			auto endTimePoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

			m_Stopped = true;

			auto duration = (end - start);

			Instrumentation::Get().WriteProfile({ m_name, start, (float)duration });
		}

	private:
		const char* m_name;
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
	};
}


#define FG_PROFILING 1
#if FG_PROFILING	
	#define FG_BEGIN_PROFILE_SESSION(name,filepath) Forge::Instrumentation::Get().BeginSession(name,filepath)
	#define FG_END_PROFILE_SESSION() Forge::Instrumentation::Get().EndSession();
	#define FG_PROFILE_SCOPE(name) Forge::Timer timer##__LINE__(name)
	#define FG_PROFILE_FUNCTION() Forge::Timer timer##__LINE__(__FUNCSIG__)
#else
	#define FG_BEGIN_SESSION(name,filepath)
	#define FG_END_SESSION(name)
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
#endif

