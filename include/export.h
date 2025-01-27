//
// Created by roteklaue on 1/27/25.
//

#ifndef CREST_EXPORT_H
#define CREST_EXPORT_H

#pragma once

#ifdef _WIN32
#define CREST_EXPORT __declspec(dllexport)
#else
#define CREST_EXPORT
#endif

#endif //CREST_EXPORT_H
