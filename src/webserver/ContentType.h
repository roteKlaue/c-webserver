//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_CONTENTTYPE_H
#define C_WEBSERVER_CONTENTTYPE_H

/**
 * @enum ContentType
 * @brief Represents different content types that can be used in HTTP responses.
 */
enum ContentType {
    JSON,  /**< Represents application/json */
    TEXT,  /**< Represents text/plain */
    HTML,  /**< Represents text/html */
    XML,   /**< Represents application/xml */
    CSS,   /**< Represents text/css */
    JS,    /**< Represents application/javascript */
    JPG,    /**< Represents image/jpeg */
    PNG,    /**< Represents image/png */
    UNKNOWN
};

/**
 * @brief Converts a ContentType enum to its corresponding string representation.
 *
 * @param content_type The ContentType enum value to convert.
 * @return A string representing the content type (e.g., "application/json").
 */
const char* ContentType_to_string(enum ContentType content_type);
enum ContentType string_to_ContentType(const char *content_type_str);

#endif //C_WEBSERVER_CONTENTTYPE_H
