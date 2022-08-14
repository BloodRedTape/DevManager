#pragma once

#include <core/list.hpp>
#include <core/string.hpp>

struct ProjectType {
	String Name;
	String MatchExpr;
	String CleanExpr;
};

struct Project {
	ProjectType* Type = nullptr;
	String Name;
	String Path;
};

struct ProjectsState {
	List<ProjectType> Types;
	List<Project> Projects;
};