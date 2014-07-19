// Copyright (C) 2013 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An object to retrieve data.

#ifndef I18N_ADDRESSINPUT_RETRIEVER_H_
#define I18N_ADDRESSINPUT_RETRIEVER_H_

#include <libaddressinput/callback.h>
#include <libaddressinput/util/basictypes.h>
#include <libaddressinput/util/scoped_ptr.h>

#include <string>

#include "lookup_key_util.h"

namespace i18n {
namespace addressinput {

class Downloader;
class Storage;
class ValidatingStorage;

// Retrieves data. Sample usage:
//    Storage* storage = ...;
//    Downloader* downloader = ...;
//    Retriever retriever("https://i18napis.appspot.com/ssl-address/",
//                        downloader, storage);
//    const scoped_ptr<const Retriever::Callback> retrieved(
//        BuildCallback(this, &MyClass::OnDataRetrieved));
//    retriever.Retrieve("data/CA/AB--fr", *retrieved);
class Retriever {
 public:
  typedef i18n::addressinput::Callback<const std::string&,
                                       const std::string&> Callback;

  // Takes ownership of |downloader| and |storage|.
  Retriever(const std::string& validation_data_url,
            const Downloader* downloader,
            Storage* storage);
  ~Retriever();

  // Retrieves the data for |key| and invokes the |retrieved| callback. Checks
  // for the data in storage first. If storage does not have the data for |key|,
  // then downloads the data and places it in storage. If the data in storage is
  // corrupted, then it's discarded and redownloaded. If the data is stale, then
  // it's redownloaded. If the download fails, then stale data will be returned
  // this one time. The next call to Retrieve() will attempt to download fresh
  // data again.
  void Retrieve(const std::string& key, const Callback& retrieved) const;

 private:
  const LookupKeyUtil lookup_key_util_;
  scoped_ptr<const Downloader> downloader_;
  scoped_ptr<ValidatingStorage> storage_;

  DISALLOW_COPY_AND_ASSIGN(Retriever);
};

}  // namespace addressinput
}  // namespace i18n

#endif  // I18N_ADDRESSINPUT_RETRIEVER_H_
