#ifndef ARCHITECTURE_HPP
#define ARCHITECTURE_HPP

#include <vector>
#include <string>

class Architecture {
public:

  bool shouldClose();

  std::vector<std::string> readFile(const char* target);

    //timer
    //Important note: Timer should be used after PlayerView is initialized
    //The benefit of using glfw's cross-platform timer outweights the interconnection

    double getDt();

    void updateTimer();

    void resetTimer();

    const static std::string texturePath;

private:

  double mCurrentTime;
  double mDt;

  Architecture();

//-----------------------------------------------------------
//Singleton implementation below:
//-----------------------------------------------------------

public:
  static Architecture & getInstance() {
    static Architecture instance;
    return instance;
  }

private:
  Architecture(Architecture const&);
  void operator=(Architecture const&);
};


#endif
