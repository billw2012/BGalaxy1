#if !defined(__MISC_FILE_FUNCS_HPP__)
#define __MISC_FILE_FUNCS_HPP__

#include <string>
#include <algorithm>

#include <Windows.h>

#define NON_STANDARD_SLASH '/'
#define STANDARD_SLASH '\\'
#define STANDARD_SLASH_STR "\\"

namespace misc {;

inline std::string standardize_path(std::string str)
{
	for(size_t idx = 0; idx < str.length(); ++idx)
	{
		if(str[idx] == NON_STANDARD_SLASH)
			str[idx] = STANDARD_SLASH;
	}
	return str;
}

// resolves all ".." elements
inline std::string resolve_all_relative_elements( std::string fullPath ) 
{
	std::string::size_type dPos = fullPath.find(STANDARD_SLASH_STR "..");
	while(dPos != std::string::npos)
	{
		std::string::size_type prevSlash = fullPath.find_last_of(STANDARD_SLASH, dPos-1);
		if(prevSlash == std::string::npos)
			prevSlash = 0;
		fullPath = fullPath.substr(0, prevSlash) + fullPath.substr(dPos+3);
		dPos = fullPath.find(STANDARD_SLASH_STR "..");
	}

	return fullPath;
}

// assumes standardized paths
inline std::string resolve_relative_path(const std::string& rootPath, 
										 const std::string& relativePath)
{
	if(relativePath.length() == 0)
		return rootPath;
	if(rootPath.length() == 0)
		return std::string("Error: Root path is empty in call to resolve_relative_path.");

	bool rootPathEndsWithSlash = rootPath[rootPath.length()-1] == STANDARD_SLASH;
	bool relativePathStartsWithSlash = relativePath[0] == STANDARD_SLASH;
	
	std::string fullPath;
	if(rootPathEndsWithSlash && relativePathStartsWithSlash)
		fullPath = rootPath + relativePath.substr(1);
	else if(!rootPathEndsWithSlash && !relativePathStartsWithSlash)
		fullPath = rootPath + STANDARD_SLASH + relativePath;
	else
		fullPath = rootPath + relativePath;

	return resolve_all_relative_elements(fullPath);
}

// concatenate two paths, adding slash in the middle if required
inline std::string concatanate_paths(const std::string& a, const std::string& b)
{
	bool bStartsWithSlash = (b.length() > 0 && b[0] == STANDARD_SLASH);
	bool aEndsWithSlash = (a.length() > 0 && a[a.length()-1] == STANDARD_SLASH);
	
	if(aEndsWithSlash && bStartsWithSlash)
		return a + b.substr(1);
	if(!aEndsWithSlash && !bStartsWithSlash)
		return a + STANDARD_SLASH + b;
	return a + b;
}

inline std::string get_full_path(std::string path, std::string cwd = "")
{
	// is path a full path?
	bool pathIsNetwork = (path.length() > 2 && path[0] == STANDARD_SLASH && path[1] == STANDARD_SLASH);

	// if its not a network path then does it have a drive specified?
	if(!pathIsNetwork && (path.length() < 2 || path[1] != ':'))
	{
		if(cwd.empty())
		{
			// if it doesn't then prepend the current working directory
			char szCwd[MAX_PATH] = "";

			if(!::GetCurrentDirectoryA(sizeof(szCwd) - 1, szCwd))
				return std::string("Error: could not get current working directory.");
			cwd = standardize_path(szCwd);
		}

		if(path.length() == 0)
			return cwd;

		path = concatanate_paths(cwd, path);
	}

	return resolve_all_relative_elements(path);
}

// assumes a full path
inline std::string get_directory(std::string path)
{
	std::string::size_type lastSlash = path.find_last_of(STANDARD_SLASH);
	if(lastSlash == std::string::npos)
		return std::string(); // it has no directory, is only a file name?
	if(lastSlash <= 2)
		return path; // it doesn't have a file name, is only a drive?

	// remove everything after the last slash
	path = path.substr(0, lastSlash);

	// resolve relative elements and return
	return resolve_all_relative_elements(path);
}

}
#endif // __MISC_FILE_FUNCS_HPP__