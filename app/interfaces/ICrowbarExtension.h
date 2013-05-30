#ifndef ICROWBAREXTENSION_H
#define ICROWBAREXTENSION_H

class QString;  // Note: The examples declare class prototypes instead of linking the headers. Not sure why but I'll do the same.
class QApplication;

// Version information for an extension.
struct
{
  int Major;    // Major version number
  int Minor;    // Minor version number
  int Revision; // Revision number
  int Build;    // Build number
} ExtVersion;

/*
 * Interface that defines the methods a Crowbar extension must implement
 */

class ICrowbarExtension
{
  // I'm still improving my familiarity with C++ at this point, so comments ahoy!
  // NTS: const functions imply they do not modify the object in any way.

public:
  virtual ~ICrowbarExtension() {}                   // Destructor is required to stop some compilers complaining.

  // Metadata
  virtual QString GetName() const = 0;              // Gets the extension name. Must be unique! 64 chars or less.
  virtual QString GetShortDescription() const = 0;  // Gets the extension description - should be 128 chars or less.
  virtual ExtVersion GetVersion() const = 0;        // Gets the extension version struct.

  // Starting up/shutting down
  virtual bool    Initialise(QApplication*) = 0;              // Called when the extension should set itself up. QApplication is the application the plugin is linked to.
                                                                // Returns true on success, false on failure.
  virtual void    Shutdown() = 0;                             // Called when the extension should shut down.
  virtual bool    IsInitialised() const = 0;                  // Returns true if Initialise has been called on the plugin and Shutdown has not.
};

#endif // ICROWBAREXTENSION_H
