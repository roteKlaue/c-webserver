//
// Created by xxrot on 04.09.2024.
//

#include "StatusCode.h"

const char* StatusCode_to_string(const enum StatusCode status_code)
{
    switch (status_code) {
        case CONTINUE:
            return "Continue";
        case SWITCHING_PROTOCOLS:
            return "Switching Protocols";
        case PROCESSING:
            return "Processing";
        case EARLY_HINTS:
            return "Early Hints";
        case OK:
            return "OK";
        case CREATED:
            return "Created";
        case ACCEPTED:
            return "Accepted";
        case NON_AUTHORITATIVE_INFORMATION:
            return "Non-Authoritative Information";
        case NO_CONTENT:
            return "No Content";
        case RESET_CONTENT:
            return "Reset Content";
        case PARTIAL_CONTENT:
            return "Partial Content";
        case MULTI_STATUS:
            return "Multi-Status";
        case ALREADY_REPORTED:
            return "Already Reported";
        case TRANSFORMATION_APPLIED:
            return "Transformation Applied";
        case THIS_IS_FINE:
            return "This Is Fine";
        case IM_USED:
            return "IM Used";
        case MULTIPLE_CHOICES:
            return "Multiple Choices";
        case MOVED_PERMANENTLY:
            return "Moved Permanently";
        case FOUND:
            return "Found";
        case SEE_OTHER:
            return "See Other";
        case NOT_MODIFIED:
            return "Not Modified";
        case USE_PROXY:
            return "Use Proxy";
        case SWITCH_PROXY:
            return "Switch Proxy";
        case TEMPORARY_REDIRECT:
            return "Temporary Redirect";
        case PERMANENT_REDIRECT:
            return "Permanent Redirect";
        case BAD_REQUEST:
            return "Bad Request";
        case UNAUTHORIZED:
            return "Unauthorized";
        case PAYMENT_REQUIRED:
            return "Payment Required";
        case FORBIDDEN:
            return "Forbidden";
        case NOT_FOUND:
            return "Not Found";
        case METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case NOT_ACCEPTABLE:
            return "Not Acceptable";
        case PROXY_AUTHENTICATION_REQUIRED:
            return "Proxy Authentication Required";
        case REQUEST_TIMEOUT:
            return "Request Timeout";
        case CONFLICT:
            return "Conflict";
        case GONE:
            return "Gone";
        case LENGTH_REQUIRED:
            return "Length Required";
        case PRECONDITION_FAILED:
            return "Precondition Failed";
        case PAYLOAD_TOO_LARGE:
            return "Payload Too Large";
        case REQUEST_URI_TOO_LARGE:
            return "Request-URI Too Large";
        case UNSUPPORTED_MEDIA_TYPE:
            return "Unsupported Media Type";
        case REQUEST_RANGE_NOT_SATISFIABLE:
            return "Request Range Not Satisfiable";
        case EXPECTATION_FAILED:
            return "Expectation Failed";
        case IM_A_TEAPOT:
            return "I'm a Teapot";
        case PAGE_EXPIRED:
            return "Page Expired";
        case ENHANCE_YOUR_CALM:
            return "Enhance Your Calm";
        case MISDIRECTED_REQUEST:
            return "Misdirected Request";
        case UNPROCESSABLE_ENTITY:
            return "Unprocessable Entity";
        case LOCKED:
            return "Locked";
        case FAILED_DEPENDENCY:
            return "Failed Dependency";
        case TOO_EARLY:
            return "Too Early";
        case UPGRADE_REQUIRED:
            return "Upgrade Required";
        case PRECONDITION_REQUIRED:
            return "Precondition Required";
        case TOO_MANY_REQUESTS:
            return "Too Many Requests";
        case REQUEST_HEADER_FIELDS_TOO_LARGE:
            return "Request Header Fields Too Large";
        case LOGIN_TIME_OUT:
            return "Login Time-Out";
        case NO_RESPONSE:
            return "No Response";
        case RETRY_WITH:
            return "Retry With";
        case BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS:
            return "Blocked by Windows Parental Controls";
        case UNAVAILABLE_FOR_LEGAL_REASONS:
            return "Unavailable For Legal Reasons";
        case CLIENT_CLOSED_CONNECTION_PREMATURELY:
            return "Client Closed Connection Prematurely";
        case TOO_MANY_FORWARDED_IP_ADDRESSES:
            return "Too Many Forwarded IP Addresses";
        case INCOMPLETE_PROTOCOL:
            return "Incomplete Protocol";
        case REQUEST_HEADER_TOO_LARGE:
            return "Request Header Too Large";
        case SSL_CERTIFICATE_ERROR:
            return "SSL Certificate Error";
        case SSL_CERTIFICATE_REQUIRED:
            return "SSL Certificate Required";
        case HTTP_REQUEST_SENT_TO_HTTPS_PORT:
            return "HTTP Request Sent to HTTPS Port";
        case TOKEN_EXPIRED_INVALID:
            return "Token Expired/Invalid";
        case CLIENT_CLOSED_REQUEST:
            return "Client Closed Request";
        case INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        case NOT_IMPLEMENTED:
            return "Not Implemented";
        case BAD_GATEWAY:
            return "Bad Gateway";
        case SERVICE_UNAVAILABLE:
            return "Service Unavailable";
        case GATEWAY_TIMEOUT:
            return "Gateway Timeout";
        case HTTP_VERSION_NOT_SUPPORTED:
            return "HTTP Version Not Supported";
        case VARIANT_ALSO_NEGOTIATES:
            return "Variant Also Negotiates";
        case INSUFFICIENT_STORAGE:
            return "Insufficient Storage";
        case LOOP_DETECTED:
            return "Loop Detected";
        case BANDWIDTH_LIMIT_EXCEEDED:
            return "Bandwidth Limit Exceeded";
        case NOT_EXTENDED:
            return "Not Extended";
        case NETWORK_AUTHENTICATION_REQUIRED:
            return "Network Authentication Required";
        case WEB_SERVER_IS_RETURNING_AN_UNKNOWN_ERROR:
            return "Web Server Is Returning An Unknown Error";
        case WEB_SERVER_IS_DOWN:
            return "Web Server Is Down";
        case CONNECTION_TIMED_OUT:
            return "Connection Timed Out";
        case ORIGIN_IS_UNREACHABLE:
            return "Origin Is Unreachable";
        case A_TIMEOUT_OCCURRED:
            return "A Timeout Occurred";
        case SSL_HANDSHAKE_FAILED:
            return "SSL Handshake Failed";
        case INVALID_SSL_CERTIFICATE:
            return "Invalid SSL Certificate";
        case RAILGUN_LISTENER_TO_ORIGIN:
            return "Railgun Listener to Origin";
        case THE_SERVICE_IS_OVERLOADED:
            return "The Service Is Overloaded";
        case SITE_FROZEN:
            return "Site Frozen";
        case NETWORK_READ_TIMEOUT_ERROR:
            return "Network Read Timeout Error";
        case NETWORK_CONNECT_TIMEOUT_ERROR:
            return "Network Connect Timeout Error";
        case REQUEST_DENIED:
            return "Request Denied";
        default:
            return "Unknown Status Code";
    }
}
