/**
 * @file policy_manager_impl.h
 * @brief Policy Manager implementation header file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_MANAGER_IMPL_H_
#define SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_MANAGER_IMPL_H_

#include "policies/policy_manager.h"
#include "interfaces/MOBILE_API.h"
#include "policies/policy_configuration.h"
#include "policies/policy_table.h"
#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"
#include "utils/logger.h"


namespace NsSmartDeviceLink {
namespace policies {


/**
 * @brief Interface implementation class of Policy Manager.
 */
class PolicyManagerImpl : public PolicyManager {
  public:
    /**
     * @brief PolicyManagerImpl is a singleton.
     *
     * @return Pointer to the single instance of PolicyManagerImpl.
     */
    static PolicyManagerImpl* instance();

    /**
     * @brief Destructor
     *
     **/
    virtual ~PolicyManagerImpl(void);

    /**
    * @brief Initialization method
    *
    * @param config PolicyManager configuration
    * 
    */
    virtual InitResult::eType Init(const PolicyConfiguration& config);

    /**
     * @brief Checking permissions for application whether rpc is allowed.
     *
     * @param app_id  Application identifier
     * @param rpc     Remote procedure call
     * @param hmi_status  HMI level status
     *
     * @return result of check permission
     */
    virtual CheckPermissionResult CheckPermission(uint32_t app_id,
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& rpc,
        const mobile_apis::HMILevel::eType hmi_status);

  protected:
    /**
     * @brief Hidden constructor.
     */
    PolicyManagerImpl();

    /**
     * @brief Store policy table to filesystem
     */
    void StorePolicyTable();

    /**
     * @brief Get PolicyTable pointer (for testing purposes)
     *
     * @return PolicyTable* ptr.
     **/
    PolicyTable* GetPolicyTable() const;

  private:
    /**
     * @brief Policy configuration
     */
    PolicyConfiguration policy_config_;

    /**
     * @brief Policy table
     */
    PolicyTable* policy_table_;

    /**
     * @brief Logger
     */
    static log4cxx::LoggerPtr logger_;
};

}  // namespace policies
}  // namespace NsSmartDeviceLink


#endif  // SRC_COMPONENTS_POLICIES_INCLUDE_POLICIES_POLICY_MANAGER_IMPL_H_
