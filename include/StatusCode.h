//
// Created by xxrot on 04.09.2024.
// Description: Defines HTTP status codes as an enum and provides a utility function
//              for converting status codes to their string representations.
//

#ifndef C_WEBSERVER_STATUSCODE_H
#define C_WEBSERVER_STATUSCODE_H

/**
 * Enum: StatusCode
 * Purpose: Enumerates standard and custom HTTP status codes.
 *          Each value represents a specific HTTP response status.
 */
enum StatusCode {
    // 1xx Informational responses
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,
    EARLY_HINTS = 103,

    // 2xx Success
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORTED = 208,
    TRANSFORMATION_APPLIED = 214, // Non-standard
    THIS_IS_FINE = 218,           // Non-standard
    IM_USED = 226,

    // 3xx Redirection
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,               // Deprecated
    SWITCH_PROXY = 306,            // Unused
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,

    // 4xx Client error responses
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    PAYLOAD_TOO_LARGE = 413,
    REQUEST_URI_TOO_LARGE = 414,   // Deprecated in favor of REQUEST_URL_TOO_LONG
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUEST_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    IM_A_TEAPOT = 418,             // Non-standard (RFC 2324)
    PAGE_EXPIRED = 419,            // Non-standard
    ENHANCE_YOUR_CALM = 420,       // Non-standard, Twitter-specific
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_ENTITY = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    TOO_EARLY = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    LOGIN_TIME_OUT = 440,          // Non-standard, Microsoft-specific
    NO_RESPONSE = 444,             // Non-standard, nginx-specific
    RETRY_WITH = 449,              // Non-standard, Microsoft-specific
    BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS = 450, // Non-standard
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    CLIENT_CLOSED_CONNECTION_PREMATURELY = 460, // Non-standard
    TOO_MANY_FORWARDED_IP_ADDRESSES = 463,      // Non-standard
    INCOMPLETE_PROTOCOL = 464,     // Non-standard
    REQUEST_HEADER_TOO_LARGE = 494,             // Non-standard, nginx-specific
    SSL_CERTIFICATE_ERROR = 495,                // Non-standard, nginx-specific
    SSL_CERTIFICATE_REQUIRED = 496,             // Non-standard, nginx-specific
    HTTP_REQUEST_SENT_TO_HTTPS_PORT = 497,      // Non-standard, nginx-specific
    TOKEN_EXPIRED_INVALID = 498,   // Non-standard
    CLIENT_CLOSED_REQUEST = 499,   // Non-standard, nginx-specific

    // 5xx Server error responses
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    BANDWIDTH_LIMIT_EXCEEDED = 509,         // Non-standard
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511,
    WEB_SERVER_IS_RETURNING_AN_UNKNOWN_ERROR = 520, // Cloudflare-specific
    WEB_SERVER_IS_DOWN = 521,               // Cloudflare-specific
    CONNECTION_TIMED_OUT = 522,             // Cloudflare-specific
    ORIGIN_IS_UNREACHABLE = 523,            // Cloudflare-specific
    A_TIMEOUT_OCCURRED = 524,               // Cloudflare-specific
    SSL_HANDSHAKE_FAILED = 525,             // Cloudflare-specific
    INVALID_SSL_CERTIFICATE = 526,          // Cloudflare-specific
    RAILGUN_LISTENER_TO_ORIGIN = 527,       // Cloudflare-specific
    THE_SERVICE_IS_OVERLOADED = 529,        // Non-standard
    SITE_FROZEN = 530,                      // Non-standard
    NETWORK_READ_TIMEOUT_ERROR = 598,       // Non-standard
    NETWORK_CONNECT_TIMEOUT_ERROR = 599,    // Non-standard

    // Miscellaneous
    REQUEST_DENIED = 999,       // Non-standard
};

/**
 * Function: StatusCode_to_string
 * Purpose: Converts an HTTP status code to its string representation.
 *
 * @param status_code - The status code to convert (from the StatusCode enum).
 * @return A string representing the status code.
 *         If the status code is unknown, returns "Unknown Status Code".
 */
const char* StatusCode_to_string(enum StatusCode status_code);

#endif //C_WEBSERVER_STATUSCODE_H
