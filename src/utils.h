#ifndef UTILS_H
#define UTILS_H

/*
 *  * If the string "str" begins with the string found in "prefix", return 1.
 *   * The "out" parameter is set to "str + strlen(prefix)" (i.e., to the point
 *   in
 *    * the string right after the prefix).
 *     *
 *      * Otherwise, return 0 and leave "out" untouched.
 *       *
 *        * Examples:
 *         *
 *          *   [extract branch name, fail if not a branch]
 *           *   if (!skip_prefix(ref, "refs/heads/", &branch)
 *            * return -1;
 *             *
 *              *   [skip prefix if present, otherwise use whole string]
 *               *   skip_prefix(name, "refs/heads/", &name);
 *                */
static inline int skip_prefix(const char *str, const char *prefix,
        const char **out)
{
    do {
        if (!*prefix) {
            *out = str;
            return 1;
        }
    } while (*str++ == *prefix++);
    return 0;
}

static inline int str_size(const char * str)
{
    int i = 0;

    if (str == NULL)
        return -1;

    for (; *(str) != '\0'; ++i, ++str);

    return i;
}
#endif
