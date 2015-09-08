//
// Polycode template. Write your code here.
//

#import "PolycodeView.h"
#include "Polycode.h"

using namespace Polycode;

class CalliperApp;

class PolycodeTemplateApp {
public:
    PolycodeTemplateApp(PolycodeView *view);
    ~PolycodeTemplateApp();
    
    bool Update();
    
private:
  CalliperApp* app;
};