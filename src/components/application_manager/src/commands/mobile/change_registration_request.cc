/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include "application_manager/commands/mobile/change_registration_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

ChangeRegistrationRequest::ChangeRegistrationRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      ui_result_(hmi_apis::Common_Result::INVALID_ENUM),
      vr_result_(hmi_apis::Common_Result::INVALID_ENUM),
      tts_result_(hmi_apis::Common_Result::INVALID_ENUM) {
}

ChangeRegistrationRequest::~ChangeRegistrationRequest() {
}

void ChangeRegistrationRequest::Run() {
  LOG4CXX_INFO(logger_, "ChangeRegistrationRequest::Run");

  ApplicationManagerImpl* instance = ApplicationManagerImpl::instance();
  const HMICapabilities& hmi_capabilities = instance->hmi_capabilities();

  ApplicationSharedPtr app = instance->application(connection_key());
  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (!hmi_capabilities.is_ui_cooperating()) {
    ui_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  if (!hmi_capabilities.is_vr_cooperating()) {
    vr_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  if (!hmi_capabilities.is_tts_cooperating()) {
    tts_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  const int32_t hmi_language =
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt();

  const int32_t language =
      (*message_)[strings::msg_params][strings::language].asInt();

  if (false ==
      (IsLanguageSupportedByUI(hmi_language) &&
       IsLanguageSupportedByVR(language) &&
       IsLanguageSupportedByTTS(language))) {
    LOG4CXX_ERROR(logger_, "Language is not supported");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  pending_requests_.Add(hmi_apis::FunctionID::UI_ChangeRegistration);
  pending_requests_.Add(hmi_apis::FunctionID::VR_ChangeRegistration);
  pending_requests_.Add(hmi_apis::FunctionID::TTS_ChangeRegistration);

  // UI processing
  smart_objects::SmartObject ui_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  ui_params[strings::language] = hmi_language;
  ui_params[strings::app_id] = app->app_id();

  SendHMIRequest(hmi_apis::FunctionID::UI_ChangeRegistration,
                 &ui_params, true);

  // VR processing
  smart_objects::SmartObject vr_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  vr_params[strings::language] =
      (*message_)[strings::msg_params][strings::language];

  vr_params[strings::app_id] = app->app_id();

  SendHMIRequest(hmi_apis::FunctionID::VR_ChangeRegistration,
                 &vr_params, true);

  // TTS processing
  smart_objects::SmartObject tts_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  tts_params[strings::language] =
      (*message_)[strings::msg_params][strings::language];

  tts_params[strings::app_id] = app->app_id();

  SendHMIRequest(hmi_apis::FunctionID::TTS_ChangeRegistration,
                 &tts_params, true);
}

bool ChangeRegistrationRequest::WasAnySuccess(
      const hmi_apis::Common_Result::eType ui,
      const hmi_apis::Common_Result::eType vr,
      const hmi_apis::Common_Result::eType tts) {

  return
      hmi_apis::Common_Result::SUCCESS == ui ||
      hmi_apis::Common_Result::SUCCESS == vr ||
      hmi_apis::Common_Result::SUCCESS == tts;
}

void ChangeRegistrationRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "ChangeRegistrationRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  hmi_apis::FunctionID::eType event_id = event.id();

  switch (event_id) {
    case hmi_apis::FunctionID::UI_ChangeRegistration: {
      LOG4CXX_INFO(logger_, "Received UI_ChangeRegistration event");
      pending_requests_.Remove(event_id);
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::VR_ChangeRegistration: {
      LOG4CXX_INFO(logger_, "Received VR_ChangeRegistration event");
      pending_requests_.Remove(event_id);
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::TTS_ChangeRegistration: {
      LOG4CXX_INFO(logger_, "Received TTS_ChangeRegistration event");
      pending_requests_.Remove(event_id);
      tts_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event_id);
      return;
    }
  }

  if (pending_requests_.IsFinal(event_id)) {
    ApplicationSharedPtr application =
        ApplicationManagerImpl::instance()->application(connection_key());

    if (!application) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      return;
    }

    if (hmi_apis::Common_Result::SUCCESS == ui_result_) {
      application->set_ui_language(static_cast<mobile_api::Language::eType>(
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt()));
    }

    if (hmi_apis::Common_Result::SUCCESS == vr_result_
        || hmi_apis::Common_Result::SUCCESS == tts_result_) {
      application->set_language(static_cast<mobile_api::Language::eType>(
          (*message_)[strings::msg_params][strings::language].asInt()));
    }

    int32_t greates_result_code = std::max(std::max(ui_result_, vr_result_),
                                       tts_result_);

    (*message_)[strings::params][strings::function_id] =
          mobile_apis::FunctionID::eType::ChangeRegistrationID;

    SendResponse(WasAnySuccess(ui_result_, vr_result_, tts_result_),
                 static_cast<mobile_apis::Result::eType>(greates_result_code),
                 NULL, &(message[strings::msg_params]));
  } else {
    LOG4CXX_INFO(logger_,
                "There are some pending responses from HMI."
                "ChangeRegistrationRequest still waiting.");
  }
}

bool ChangeRegistrationRequest::IsLanguageSupportedByUI(
    const int32_t& hmi_display_lang) {

  const HMICapabilities& hmi_capabilities =
      ApplicationManagerImpl::instance()->hmi_capabilities();
  const smart_objects::SmartObject* ui_languages =
      hmi_capabilities.ui_supported_languages();

  if (!ui_languages) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return false;
  }

  for (size_t i = 0; i < ui_languages->length(); ++i) {
    if (hmi_display_lang == ui_languages->getElement(i).asInt()) {
      return true;
    }
  }

  LOG4CXX_ERROR(logger_, "Language isn't supported by UI");

  return false;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByVR(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities =
      ApplicationManagerImpl::instance()->hmi_capabilities();
  const smart_objects::SmartObject* vr_languages =
      hmi_capabilities.vr_supported_languages();

  if (!vr_languages) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return false;
  }

  for (size_t i = 0; i < vr_languages->length(); ++i) {
    if (hmi_display_lang == vr_languages->getElement(i).asInt()) {
      return true;
    }
  }

  LOG4CXX_ERROR(logger_, "Language isn't supported by VR");

  return false;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByTTS(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities =
      ApplicationManagerImpl::instance()->hmi_capabilities();
  const smart_objects::SmartObject* tts_languages =
      hmi_capabilities.tts_supported_languages();

  if (!tts_languages) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return false;
  }

  for (size_t i = 0; i < tts_languages->length(); ++i) {
    if (hmi_display_lang == tts_languages->getElement(i).asInt()) {
      return true;
      break;
    }
  }

  LOG4CXX_ERROR(logger_, "Language isn't supported by TTS");
  return false;
}

}  // namespace commands

}  // namespace application_manager
