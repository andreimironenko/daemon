//
// Created by amironenko on 31/10/2020.
//
/**!
 * The Linux/Unix daemon baseclass
 *
 *
 * // Command lines:
 * --work-directory
 * --keep-open-file-descriptors
 * --keep-std-streams
 * --no-umask-update
 * --max-file-descriptor (to close. default 8192)
 */

#pragma once
#include <memory>
#include <string>
//namespace daemon {
    // Forward declarations;
class core {

  class core_;
  std::unique_ptr<core_> _core;


  public:
  explicit core(std::string name, int argc, char** argv);

  virtual ~core();

  core(core&&) = delete;
  core(const core&) = delete;
  core& operator=(core&&) = delete;
  core& operator=(const core&) = delete;

};
//} //namespace daemon


