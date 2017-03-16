#ifndef __LOWI_RESPONSE_H__
#define __LOWI_RESPONSE_H__

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

        LOWI Response Interface Header file

GENERAL DESCRIPTION
  This file contains the structure definitions and function prototypes for
  LOWIResponse

Copyright (c) 2012 Qualcomm Atheros, Inc.
  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.
=============================================================================*/
#include <inc/lowi_const.h>
#include <inc/lowi_scan_measurement.h>


namespace qc_loc_fw
{

/** Events generated by the WiFi driver as a result of background scan
 *  requests
 */
enum eLOWIDriverEvent
{
  /** Signifies no event */
  LOWI_NO_EVT = 0,
  /** Reported when report_threshold is reached in scan cache */
  LOWI_ON_SCAN_RESULTS_AVAILABLE_EVT = 1,
  /**
   *  Reported by the wifi driver when each probe response is
   *  received, if reportEvents enabled in LOWIScanParams
   */
  LOWI_ON_FULL_SCAN_RESULT_EVT = 2,
  /**
   *  Optional event reported by the wifi driver - indicates progress
   *  of scanning state machine
   */
  LOWI_ON_PROGRESS_SCAN_EVT = 3,
  /** Reported by the wifi driver when an AP from a hotlist has been found */
  LOWI_AP_FOUND_EVT = 4,
  /** Reported by the wifi driver when an AP from a hotlist has been lost */
  LOWI_AP_LOST_EVT = 5,
  /**
   *  Reported by the wifi driver when a significant change has occurred on
   *  an AP that is on the significant change list
   */
  LOWI_SIGNIFICANT_CHANGE_EVT = 6,
  /** Unknown event reported by the wifi driver */
  LOWI_UNKNOWN_EVT,
};

/** Event generated by WiFi driver to provide a progress update on a
 *  background scan
 */
enum eLOWIBgScanEvent
{
  LOWI_SCAN_EVT_BUFFER_FULL = 0,
  LOWI_SCAN_EVT_COMPLETE    = 1,
  LOWI_SCAN_EVT_NONE        = 2
};

// Forward declaration
class LOWIRequest;

/**
 * Base class for Response
 */
class LOWIResponse
{
private:
  uint32 requestId;
public:
  /**
   * Log Tag
   */
  static const char * const TAG;

  /** Type of Response
   * When adding additional responses, update the following utility function:
   *  -- LOWIUtils::to_string(LOWIResponse::eResponseType a)
   *
   *  ADD NEW RESPONSES ONLY AT THE END OF THIS ENUM
   */
  enum eResponseType
  {
    /** Unknown response type */
    RESPONSE_TYPE_UNKNOWN = 0,
    /** Response carrying the results produced by a DISCOVERY_SCAN request */
    DISCOVERY_SCAN,
    /** Response carrying the results produced by a RANGING_SCAN request */
    RANGING_SCAN,
    /** List of capabilities supported by LOWI */
    CAPABILITY,
    /** Response to a RESET_CACHE request */
    RESET_CACHE,
    /** Response carrying asynchronous results produced by an
     *  ASYNC_DISCOVERY_SCAN_RESULTS request */
    ASYNC_DISCOVERY_SCAN_RESULTS,
    /** Status response */
    LOWI_STATUS,
    /** FW background scan capabilities */
    BGSCAN_CAPABILITIES,
    /** List of frequency channels supported by the WiFi driver */
    BGSCAN_CHANNELS_SUPPORTED,
    /** Asynchronous response event resulting from a
     *  SIGNIFINCANT_CHANGE_LIST_SET request */
    BGSCAN_EVENT,
    /** Response carrying cached results produced a BGSCAN_START request */
    BGSCAN_CACHED_RESULTS,
    /** Asynchronous response event resulting from a HOTLIST_SET request */
    HOTLIST_AP_EVENT,
    /** Asynchronous response event resulting from a
     *  SIGNIFINCANT_CHANGE_LIST_SET request */
    SIGNIFICANT_CHANGE_EVENT,
    /** Response to the capability subscription request, carries dynamic capabilities*/
    CAPABILITY_SUBSCRIPTION,
    /** Response carrying cached results produced a BGSCAN_START request per
     *  new M-release
     */
    BGSCAN_CACHED_RESULTS_VECTOR,
  };

  /**
   * Defines status of the Scan Request.
   */
  enum eScanStatus
  {
    SCAN_STATUS_UNKNOWN = 0,
    /** Measurements were obtained successfully from the WLAN driver.
     * Note that SUCCESS does not guarantee that there is at least one
     * measurement in this packet. It is possible to have zero measurement
     * and a SUCCESS; if there are no APs in the vicinity.*/
    SCAN_STATUS_SUCCESS = 1,
    /** Indicates that the number of pending clients have reached the maximum*/
    SCAN_STATUS_BUSY = 2,
    /** Unable to initiate request to driver*/
    SCAN_STATUS_DRIVER_ERROR = 3,
    /** Unable to get response from driver*/
    SCAN_STATUS_DRIVER_TIMEOUT = 4,
    /** There is an internal error condition that causes
     * LOWI unable to provide any measurements*/
    SCAN_STATUS_INTERNAL_ERROR = 5,
    /** Invalid request*/
    SCAN_STATUS_INVALID_REQ = 6,
    /** Request not supported */
    SCAN_STATUS_NOT_SUPPORTED = 7,
    /** Wifi not enabled */
    SCAN_STATUS_NO_WIFI = 8,
    /** Too many instances of this request type */
    SCAN_STATUS_TOO_MANY_REQUESTS = 9,
    /** Out of memory condition */
    SCAN_STATUS_OUT_OF_MEMORY = 10
  };

  /**
   * Constructor
   * @param uint32 requestId generated by the client for request
   */
  LOWIResponse (uint32 requestId);
  /**
   * Destructor
   */
  virtual ~LOWIResponse () = 0;

  /**
   * Request id generated and provided in request by the client.
   * Echoed back in the response.
   * @return Corresponding Request id
   */
  uint32 getRequestId ();

  /**
   * Returns the response type
   * @return eResponseType Type of Response
   */
  virtual eResponseType getResponseType () = 0;
};

/**
 * This class defines the Capabilities of the WifiDriver
 */
class LOWICapabilities
{
public:
  /** true if Discovery scan is supported*/
  bool discoveryScanSupported;

  /** true if Ranging scan is supported*/
  bool rangingScanSupported;

  /** true if Active scan is supported*/
  bool activeScanSupported;

 /** true if single-sided ranging is supported */
  bool oneSidedRangingSupported;

  /** true if dual-sided ranging per 11v std is supported */
  bool dualSidedRangingSupported11v;

  /** true if dual-sided ranging per 11mc std is supported */
  bool dualSidedRangingSupported11mc;

  /** true if bgscan-compatible scan off-loading is supported */
  bool bgscanSupported;

  /** Highest bandwidth support for rtt requests */
  uint8 bwSupport;

  /** Bit mask representing preambles supported for rtt requests */
  uint8 preambleSupport;

  //driver static capability bit masks
  #define LOWI_NO_SCAN_SUPPORTED 0x00
  #define LOWI_LP_SCAN_SUPPORTED 0x01
  #define LOWI_DISCOVERY_SCAN_SUPPORTED 0x02
  #define LOWI_RANGING_SCAN_SUPPORTED 0x04
  #define LOWI_BG_SCAN_SUPPORTED 0x08

  /** Bit mask representing capability supported for the loaded driver
   * look for below bitmasks for the supported capablity.
   * LOWI_NO_SCAN_SUPPORTED,LOWI_LP_SCAN_SUPPORTED,
   * LOWI_DISCOVERY_SCAN_SUPPORTED,LOWI_RANGING_SCAN_SUPPORTED
   * LOWI_BG_SCAN_SUPPORTED */
  uint32 supportedCapablities;

public:
  /**
   * Constructor
   */
  LOWICapabilities ();
};

/**
 * This class provides the dynamic capabilities discovered
 * by communicating with different modules. i.e. Capabilities of LOWI-LP
 */
class LOWIDynamicCapabilities
{
public:
  /** Batching support from LOWI-LP */
  bool lpBatchingSupported;
  /** Max APs that could be batched through LOWI-LP*/
  uint32 lpMaxNumAPBatched;
  /** bgscan batching support from LOWI-LP */
  bool lpBgscanBatchingSupported;
  /** total memory allocated for bgscan batching (in bytes) */
  uint32 lpBgscanMaxScanCacheSize;
  /** maximum number of wifi nodes to be cached per bgscan */
  uint32 lpBgscanMaxApCachePerScan;
  /** maximum % threshold allowed in bgscan_start params: LOWIScanParams.
   *  LOWI-LP supports 90% max */
  uint32 lpBgscanMaxScanReportThresh;
  /**
   * Constructor
   */
  LOWIDynamicCapabilities ();
};

/**
 * Response to the Capability Request
 */
class LOWICapabilityResponse : public LOWIResponse
{
private:
  LOWICapabilities mCapabilities;
  bool             mStatus;
public:
  /**
   * Constructor
   * @param uint32 requestId generated by the client for request
   * @param LOWICapabilities Capabilities object
   * @param bool true for success, false to indicate failure
   */
  LOWICapabilityResponse (uint32 requestId, LOWICapabilities capabilities, bool status);
  /** Destructor*/
  virtual ~LOWICapabilityResponse ();

  /**
   * Returns the response type
   * @return eResponseType Type of Response
   */
  virtual eResponseType getResponseType ();

  /**
   * Return the capabilities
   * @return LOWICapabilities
   */
  LOWICapabilities getCapabilities ();

  /**
   * Returns the status of the Request
   * @return bool true for success, false otherwise
   */
  bool getStatus ();

};

/**
 * Response to the Dynamic Capability Request
 */
class LOWICapabilitySubscriptionResponse : public LOWIResponse
{
private:
  LOWIDynamicCapabilities mCapabilities;
  bool                    mStatus;
public:
  /**
   * Constructor
   * @param uint32 requestId generated by the client for request
   * @param LOWIDynamicCapabilities Capabilities object
   * @param bool true if able to provide the capabilities, false otherwise
   */
  LOWICapabilitySubscriptionResponse (uint32 requestId,
      LOWIDynamicCapabilities capabilities, bool status);

  /** Destructor*/
  virtual ~LOWICapabilitySubscriptionResponse ();

  /**
   * Returns the response type
   * @return eResponseType Type of Response
   */
  virtual eResponseType getResponseType ();

  /**
   * Return the capabilities
   * @return LOWIDynamicCapabilities
   */
  LOWIDynamicCapabilities getCapabilities ();

  /**
   * Returns the status of the Request
   * @return bool true for success, false otherwise
   */
  bool getStatus ();

};


/**
 * Response to the Cache reset Request
 */
class LOWICacheResetResponse : public LOWIResponse
{
private:
  bool mCacheResetStatus;
public:
  /**
   * Constructor
   * @param uint32 requestId generated by the client for request
   * @param bool true for success, false to indicate failure
   */
  LOWICacheResetResponse (uint32 requestId, bool status);
  /** Destructor*/
  virtual ~LOWICacheResetResponse ();

  /**
   * Returns the response type
   * @return eResponseType Type of Response
   */
  virtual eResponseType getResponseType ();

  /**
   * Returns the status of the Request
   * @return bool true for success, false otherwise
   */
  bool getStatus ();
};

/**
 * Response to the Discovery Scan Request
 */
class LOWIDiscoveryScanResponse : public LOWIResponse
{
public:
  /** Response type*/
  enum eScanTypeResponse
  {
    WLAN_SCAN_TYPE_UNKNOWN = 0,
    WLAN_SCAN_TYPE_PASSIVE,
    WLAN_SCAN_TYPE_ACTIVE
  };

  /** This may be different than the requested ScanType,
   * since user may request active scan, but if there�s an ongoing
   * passive scan results, LOWI may give back those results.*/
  eScanTypeResponse       scanTypeResponse;
  /** Status of the scan */
  eScanStatus             scanStatus;
  /** Time at the end of scan � should be equal to the timestamp of the
   * latest measurement in this packet. If there are zero measurements,
   * timestamp is the time when the response from the driver is received.
   * In msec, since January 1, 1970 UTC
   */
  uint64                   timestamp;
  /** Dynamic array containing received ScanMeasurement*/
  vector <LOWIScanMeasurement*> scanMeasurements;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated by the client for request
   */
  LOWIDiscoveryScanResponse (uint32 requestId);
  /** Destructor*/
  virtual ~LOWIDiscoveryScanResponse ();

  /**
   * Returns the response type
   * @return eResponseType type of Response
   */
  virtual eResponseType getResponseType ();
};

/**
 * Response to the Ranging scan Request
 */
class LOWIRangingScanResponse: public LOWIResponse
{
public:
  /** Status of the scan */
  eScanStatus             scanStatus;
  /** Dynamic array containing received ScanMeasurement*/
  vector <LOWIScanMeasurement*> scanMeasurements;

  /**
   * Constructor
   * @param uint32 Request Id generated by the client for Request
   */
  LOWIRangingScanResponse (uint32 requestId);
  /** Destructor*/
  virtual ~LOWIRangingScanResponse ();

  /**
   * Returns the response type
   * @return eResponseType type of Response
   */
  virtual eResponseType getResponseType ();
};

class LOWIAsyncDiscoveryScanResultResponse :
  public LOWIDiscoveryScanResponse
{
public:
    /**
     * Constructor
     * @param uint32 Corresponding Request Id generated
     *               by the client for request
     */
    LOWIAsyncDiscoveryScanResultResponse (uint32 requestId);
    /** Destructor*/
    virtual ~LOWIAsyncDiscoveryScanResultResponse ();

    /**
     * Returns the response type
     * @return eResponseType type of Response
     */
    virtual eResponseType getResponseType ();
};

/** Status response from the wifi driver to a given
 *  background scan request (bgscan, hotlist, etc).
 */
class LOWIStatusResponse : public LOWIResponse
{
public:
  /** Status returned by lower layer */
  eScanStatus scanStatus;

  /** Request type for which this is a status response */
  LOWIRequest::eRequestType mRequestType;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWIStatusResponse(uint32 requestId);
  /** Destructor*/
  ~LOWIStatusResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

/** Gscan capabilites retrieved from the FW */
class LOWIGscanCapabilities
{
public:
  /** total space allocated for scan (in bytes) */
  int32 maxScanCacheSize;
  /** maximum number of channel buckets */
  int32 maxScanBuckets;
  /** maximum number of APs that can be stored per scan  */
  int32 maxApCachePerScan;
  /** number of RSSI samples used for averaging RSSI */
  int32 maxRssiSampleSize;
  /** max possible report_threshold for providing results availability */
  int32 maxScanReportingThres;
  /** maximum number of entries for hotlist BSSIDs */
  int32 maxHotlistAps;
  /** maximum number of entries for hotlist SSIDs */
  int32 maxHotlistSsids;
  /** maximum number of entries for significant wifi change APs */
  int32 maxSignificantChangeAps;
  /** number of BSSID/RSSI entries that device can hold */
  int32 maxBssidHistoryEntries;
  /** maximum number of epno entries */
  int32 maxNumEpnoNetworks;
  /** max number of epno entries if ssid is specified that is, epno entries
   *  for which an exact match is required, or entries corresponding to
   *  hidden ssids
   */
  int32 maxNumEpnoNetworksBySSID;
  /** maximum numver of white listed SSIDs, M target is 2 to 4 */
  int32 maxNumWhiteListedSSIDs;
};

/**
 * Response for bgscan capabilities request
 */
class LOWIGscanCapsResponse : public LOWIResponse
{

public:
  /** Status returned by WiFi driver */
  eScanStatus scanStatus;

  /** bgscan capabilities */
  LOWIGscanCapabilities mCaps;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWIGscanCapsResponse(uint32 requestId);

  /** Destructor*/
  ~LOWIGscanCapsResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

/**
 * Response containing the frequency channels supported by the wifi driver
 */
class LOWIChannelsSupportedResponse : public LOWIResponse
{
public:
  /** Status returned by WiFi driver */
  eScanStatus scanStatus;

  /** dynamic array containing a list of wifi driver supported channels */
  vector <LOWIScanMeasurement*> scanMeasurements;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWIChannelsSupportedResponse(uint32 requestId);
  /** Destructor*/
  ~LOWIChannelsSupportedResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

/**
 * This response contains the actual batched scan results that have been
 * buffered in the FW.
 */
class LOWIGscanCachedResultsResponse : public LOWIResponse
{
public:
  /** Status returned by WiFi driver */
  eScanStatus scanStatus;

  /** Dynamic array containing the bgscan cached scan results */
  vector <LOWIScanMeasurement*> scanMeasurements;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWIGscanCachedResultsResponse(uint32 requestId);
  /** Destructor*/
  ~LOWIGscanCachedResultsResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

/**
 * This response contains the actual batched scan results that have been
 * buffered in the FW or by LOWI-LP. This class is needed as a result of
 * how the cached results are retrieved in the M-release code.
 */
class LOWIBGscanCachedResultsResponse : public LOWIResponse
{
public:
  /** Status returned by WiFi driver */
  eScanStatus scanStatus;

  /** Dynamic array containing the bgscan cached scan results */
  vector <LOWIBgscanCachedResult*> cachedResults;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWIBGscanCachedResultsResponse(uint32 requestId);
  /** Destructor*/
  ~LOWIBGscanCachedResultsResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

/**
 * This response carries the asynchronous results sent by the driver as a
 * result of a BGSCAN_START request. Three different events are serviced by
 * this response:
 * -- GSCAN_RESULTS_AVAILABLE
 * -- GSCAN_FULL_RESULTS
 * -- GSCAN_EVENT
 * Not all the fields in the response are needed for every event so only the
 * needed fields will be populated.
 */
class LOWIGscanEventsResponse : public LOWIResponse
{

public:
  /** Event generated by WiFi driver */
  eLOWIDriverEvent evt;
  /** Number of results available -- pertains to
   *  LOWI_ON_SCAN_RESULTS_AVAILABLE_EVT event */
  uint32 numResultsAvailable;
  /** scan results -- pertains to LOWI_ON_FULL_SCAN_RESULT_EVT event */
  vector<LOWIScanMeasurement*> scanMeasurements;
  /** Progress scan event -- pertains to LOWI_ON_PROGRESS_SCAN_EVT event */
  eLOWIBgScanEvent progressScanEvt;
  /** Progress scan event status -- pertains to LOWI_ON_PROGRESS_SCAN_EVT
   *  event */
  uint8 progressScanEvtStatus;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWIGscanEventsResponse(uint32 requestId);
  /** Destructor*/
  ~LOWIGscanEventsResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

/**
 * This response results from the detection by the wifi driver of an AP that is
 * in the hotlist passed in the HOTLIST_SET request.
 */
class LOWIHotlistApEventResponse : public LOWIResponse
{
public:
  /** Event generated by WiFi driver */
  eLOWIDriverEvent evt;
  /** Dynamic array containing the data for a hotlist AP that has been
   *  detected
   */
  vector<LOWIScanMeasurement*> scanMeasurements;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWIHotlistApEventResponse(uint32 requestId);
  /** Destructor*/
  ~LOWIHotlistApEventResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

/**
 * This response results from the detection by the wifi driver of an AP that is
 * in the significant change list passed in the SIGNIFINCANT_CHANGE_LIST_SET
 * request.
 */
class LOWISignificantChangeResponse : public LOWIResponse
{

public:
  /** Event generated by WiFi driver */
  eLOWIDriverEvent evt;
  /** Dynamic array containing the data for a AP in the
   *  significant change list that has been detected
   */
  vector<LOWIScanMeasurement*> scanMeasurements;

  /**
   * Constructor
   * @param uint32 Corresponding Request Id generated
   *               by the client for request
   */
  LOWISignificantChangeResponse(uint32 requestId);
  /** Destructor*/
  ~LOWISignificantChangeResponse();

  /**
   * Returns the response type
   * @return eResponseType: type of response
   */
  virtual eResponseType getResponseType();
};

} // namespace qc_loc_fw

#endif //#ifndef __LOWI_RESPONSE_H__